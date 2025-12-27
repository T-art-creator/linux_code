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
    TreeNode* build(std::vector<int>:: iterator inbegin,std::vector<int>:: iterator inend,std::vector<int> ::iterator psbegin,std::vector<int> ::iterator psend)
    {
        if(psbegin==psend) return nullptr;//空树

        //获取根
        TreeNode* root=new TreeNode(*(psend-1));

        //构造左子树

        int left_num=0;
        auto  begin=inbegin;
        while(*begin!=root->val)
        {
            left_num++;
            ++begin;
        }
        TreeNode* left=build(inbegin,inbegin+left_num,psbegin,psbegin+left_num);
        
        //构造右子树

        TreeNode* right=build(inbegin+left_num+1,inend,psbegin+left_num,psbegin+left_num+(psend-psbegin-1-left_num));

        //链接三部分
        root->left=left;
        root->right=right;
        return root;
    }
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) 
    {
        TreeNode* root=build(inorder.begin(),inorder.end(),postorder.begin(),postorder.end());
        return root;
    }
};