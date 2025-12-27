//https://www.nowcoder.com/practice/947f6eb80d944a84850b0538bf0ec3a5?tpId=13&&tqId=11179&rp=1&ru=/activity/oj&qru=/ta/coding-interviews/question-ranking
//算法题

/*
struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
	TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
	}
};*/
#include <ios>
class Solution {
public:

	void ConvertHelper(TreeNode* cur,TreeNode*& pre)//pre随着递归要更新 采用引用
	{
		//递归出口
		if(cur==nullptr) return;//递归到某分支最小结点或者最大结点开始回溯
		//中序遍历 左 根 右
		ConvertHelper(cur->left,pre);
		//根 -> 重构链接关系
		cur->left=pre;// 相当于cur->_pre=pre
		if(pre) pre->right=cur;//处理pre 为空结点的情况 // 相当于_pre->_next=cur
		pre=cur;//更新pre
		//右
		ConvertHelper(cur->right,pre);


	}

    TreeNode* Convert(TreeNode* pRootOfTree) 
	{
        //二叉搜索树和双向链表的底层设计结构 无非就是两指针和一个数值
		//结合二叉搜索树左小 根 中 右大 的特性满足有序双向链表的构造（只改变链接关系）
		//只需将left 改成 _pre  right 改成 _next
		//全局来看 可采用中序遍历的递归思想 使得该步改造的前提 前面的小数据已经改好了

		if(pRootOfTree==nullptr) return nullptr;
		//定义指针
		TreeNode* pre=nullptr;//相对根节点来说的上一个结点 （中序遍历回溯或者往下递归时）即有序双向链表_pre
		// 辅助递归函数
		 ConvertHelper(pRootOfTree,pre);//传当前结点和上一个结点

		//传回 有序双向链表的任意一个端口结点指针，即最小 或者最大

		//传最小 即 二叉搜索树最左
		TreeNode* ret=pRootOfTree;
		while(ret->left)
		{
			ret=ret->left;
		}
		return ret;

    }
};
