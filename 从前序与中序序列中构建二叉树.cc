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
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) 
    {
        //思路：
        //递归 将二叉树的建立 分为 左子树 根  右子树 三部分构建
        //1. 通过前序遍历 获得根
        //2，结合中序遍历和根 分别获得左右子树的前中序遍历结果 继续递归构建 知道 递归到空树

        //判断当前树是否为空树 
        if(!preorder.size()) return nullptr;//前序遍历为空 没有根节点

        //拿根
        TreeNode*  root=new TreeNode(preorder[0]);
        //构造左子树

        // 计算左右子树的结点个数
        int left_num=0; 
        for(left_num=0;inorder[left_num]!=root->val;left_num++){}
        int right_num=inorder.size()-1-left_num;
        
        //1 .拿到左子树的前序遍历结果

        //从当前树的前序遍历中拿到左子树即可

        auto Left_Pre_begin=preorder.begin()+1;//根+1
        auto Left_Pre_end=Left_Pre_begin+left_num;
        vector<int> Left_Pre_Tree(Left_Pre_begin,Left_Pre_end);//迭代区间构造 


        //2 ，拿到左子树的中序遍历结果

        //从当前树的 中序遍历中拿到左子树即可

        auto Left_In_begin=inorder.begin();
        auto Left_In_end=Left_In_begin+left_num;//根的位置
        vector<int> Left_In_Tree(Left_In_begin,Left_In_end);

        TreeNode* left=buildTree(Left_Pre_Tree,Left_In_Tree);
        //构造右子树

        //1. 拿到右子树的前序遍历结果

        //从前序遍历的结果中拿到右子树即可

        auto  Right_Pre_begin=Left_Pre_end;//右子树的第一个结点位置 
        auto Right_Pre_end=preorder.end();
        vector<int> Right_Pre_Tree(Right_Pre_begin,Right_Pre_end);


        //2. 2拿到哟字数的中序遍历结果

        //从当前树的中序遍历的结果中拿到右子树即可  

        auto Right_In_begin=Left_In_end+1;//中序遍历根的下一个位置
        auto Right_In_end=Right_In_begin+right_num;//相当于 Inorder.end()
        vector<int> Right_In_Tree(Right_In_begin,Right_In_end);

        TreeNode* right=buildTree(Right_Pre_Tree,Right_In_Tree);

        //根链接 左右子树
        if(left) root->left=left;
        if(right)root->right=right;

        return root;
    }
};


// v2 减少了 buildTree 中vector的拷贝  直接通过迭代器的调用来访问一片区间的内容

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
    TreeNode* build(std::vector<int>::iterator prebegin,std::vector<int>::iterator preend,std::vector<int>::iterator inbegin,std::vector<int>::iterator inend)
    {
        if(prebegin==preend) return nullptr;//空树

        //拿去根
        TreeNode* root=new TreeNode(*prebegin);

        //构造左子树
        int left_num=0;//左子树的个数
        auto begin=inbegin;
        while(*begin!=root->val) 
        {
            left_num++;
            ++begin;
        }

        TreeNode* left=build(prebegin+1,prebegin+1+left_num,inbegin,inbegin+left_num);


        //构造右子树

        TreeNode* right=build(prebegin+1+left_num,preend,inbegin+left_num+1,inend);
        
        //链接三者
         root->left=left;
         root->right=right;
        return root;

    }
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) 
    {
        // v2 优化 通过传参迭代器 来访问一片区域的内容
        TreeNode* root=  build(preorder.begin(),preorder.end(),inorder.begin(),inorder.end());
        return root;
    }
};

