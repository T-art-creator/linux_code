/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    string tree2str(TreeNode* root) 
    {
        //为什么这种利用 二次压栈 和  后进先出的特性  可以模拟树的递归以及结果整体 (val)形式的正确性？？

        //解答第一个问题：
        //递归的本质 就是 先 递  再 归 
        //以本题 先序遍历为例子   根- 左  - 右  本身的流程 就是 处理根 ->  处理左子树  -> 左子树回溯到根 -> 处理右子树  -> 右子树回溯到根
        //因此 后进先出  和二次压栈 可以保证处理 子树后 再归到根处 处理根

        //解答第二个问题：
        //以最后一个 非root 结点为例 假如他没有子树 第一次访问 它完成 ( val  因为二次压栈，第二次访问可以完成)  最后总体完成了(val)
        //两个 非root 结点为例子  分别为 a ,b  整体完成情况为 (a    ->>>   (a(b  ->>>  (a(b)  ->>> (a(b))  
        //扩展 此时把b 想成子树 把 a 想成根 因为栈的后进先出特性 最后必然会先处理左右子树的问题 后 处理根的 )  总而保证了 ) 不是扩一个val  而是 扩了许多儿孙结点的(val)



        //本题利用栈 和 哈希 两者的迭代 来模拟 二叉树的先序遍历（递归）的过程

        string ret="";
        //栈
        stack<TreeNode*> st;//利用 栈的后进先出的特性 + 二次压栈 模拟递 和 归  的过程 
        unordered_set<TreeNode*> s;//统计一个结点 是否访问过 从而 实现 二次访问 即递归中回溯 的过程
        st.push(root);
        while(!st.empty())
        {
            //获取栈顶数据
            TreeNode* node=st.top();
            st.pop();//凡是利用后进先出 这种特性  使用栈这种结构的应用 这两部操作必须绑定 否则易引起 栈的无限压栈 造成死循环
                    //栈的应用设计思路：  获得栈顶 出栈 处理栈顶数据
            //判断当前结点是否首次访问 即 处在递的过程还是回溯 归的过程
            if(s.count(node))
            {
                //非首次访问，回溯 即 归的过程
                //只需 + ） 即可
                //root 不需
                if(node!=root) ret+=")";
            }
            else
            {
                //首次访问 即 递归中的递  
                s.insert(node);//标记
                if(node!=root) ret+="(";
                ret+=to_string(node->val);

                //二次压栈
                st.push(node);//核心代码  实现归的过程 

                //判断是否有无法省略的空结点打印情况 
                if(!node->left && node->right) ret+="()";

                //模拟 先序  即 根 左 右  集合后进先出 的特性 先右子树 压栈 再左子树 压栈
                if(node->right) st.push(node->right);
                if(node->left) st.push(node->left);
            }
        }

        //结果
        return ret;

    }
};