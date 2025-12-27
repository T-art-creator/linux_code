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
    vector<vector<int>> levelOrder(TreeNode* root) 
    {
        //BFS 只不过 将每一层的数据记录到vector中  
        //VFS queue的特性：每一层走完后 进入下一层时可以直接拿到该层的所有个数 通过个数来记录每一层结点的数据

        vector<vector<int>> result;//二维数据 每一行即为 树 每一层的数据
        queue<TreeNode*> q;
        if(root!=nullptr) q.push(root);//入队
        while(!q.empty())
        {
            //记录当前层的数据
            int size=q.size();
            vector<int> v;//统计当前层的数据
            for(int i=0;i<size;i++)
            {
                //拿到当前结点数据
                TreeNode* cur=q.front();
                q.pop();
                v.push_back(cur->val);//统计该节点的数据
                //孩子入队
                if(cur->left) q.push(cur->left);
                if(cur->right) q.push(cur->right);
           }
            //把当前层的数据插进二维数组里
            result.push_back(v);
        }
        return result;
    }
};