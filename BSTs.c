#include<stdio.h>
#include<stdlib.h>
#include"NoteSynth.c"

typedef struct BST_Node_Struct
{
    double key;
    double freq;
    int bar;
    double index;
	struct BST_Node_Struct *left;
	struct BST_Node_Struct *right;
  
} BST_Node;

BST_Node *newBST_Node(double freq, int bar, double index)
{
	BST_Node *new_node = NULL;
	new_node = (BST_Node*)calloc(1,sizeof(BST_Node));
	new_node->freq = freq;
	new_node->bar = bar;
	new_node->index = index;
	new_node->key = (10.0*bar) + index;
	new_node->left = NULL;
	new_node->right = NULL;
	return new_node;       
}

BST_Node *BST_insert(BST_Node *root, BST_Node *new_node)
{
	if(root==NULL){
		root = new_node;
		return root;
	}
	if(root->key > new_node->key){
		root->left = BST_insert(root->left, new_node);
		return root;
	}
	if(root->key < new_node->key){
		root->right = BST_insert(root->right, new_node);
		return root;
	}
	if(root->key == new_node->key){
		printf("Duplicate node requested (bar:index)=%d,%lf, it was ignored\n",root->bar, root->index);
		return root;
	}
	return NULL;
}

BST_Node *BST_search(BST_Node *root, int bar, double index)
{
	double wanted = 10.0*bar + index;
	if(root == NULL){
		return NULL;
	}
	if(root->key == wanted){
		return root;
	}
	else if(root->key > wanted){
		return BST_search(root->left, bar, index);
	}
	else if(root->key < wanted){
		return BST_search(root->right, bar, index);
	}
	return NULL;
}

BST_Node *find_successor(BST_Node *right_child_node)
{
	if(right_child_node==NULL){
		return NULL;
	}
	while(right_child_node->left!=NULL){
		right_child_node = right_child_node->left;
	}
	return right_child_node;
}

BST_Node *BST_delete(BST_Node *root, int bar, double index)
{
	if(root==NULL){
		return NULL;
	}
	double key = (10.0*bar) + index;
	if(root->key==key){
		if(root->left==NULL && root->right==NULL){
			free(root);
			return NULL;
		}
		else if(root->left==NULL){
			BST_Node *temp = root->right;
			free(root);
			return temp;
		}
		else if(root->right==NULL){
			BST_Node *temp = root->left;
			free(root);
			return temp;
		}
		else{
			BST_Node *successor = find_successor(root->right);
			root->freq = successor->freq;
			root->index = successor->index;
			root->bar = successor->bar;
			root->key = successor->key;
			root->right = BST_delete(root->right, successor->bar, successor->index);
			return root;
		}
	}
	if(root->key > key){
		root->left = BST_delete(root->left, bar, index);
	}
	if(root->key < key){
		root->right = BST_delete(root->right, bar, index);
	}
	return root;
}

void BST_makePlayList(BST_Node *root)
{
	if(root->left!=NULL){
		BST_makePlayList(root->left);
	}
	playlist_head = playlist_insert(playlist_head,root->freq,root->bar,root->index);
    if(root->right!=NULL){
		BST_makePlayList(root->right);
	}
}

void BST_inOrder(BST_Node *root, int depth)
{
	depth+=1;
	if(root->left!=NULL){
		BST_inOrder(root->left, depth);
	}
	depth-=1;
	printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
	depth+=1;
    if(root->right!=NULL){
		BST_inOrder(root->right, depth);
	depth-=1;
	}
} 

void BST_preOrder(BST_Node *root, int depth)
{
	printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
	depth+=1;
	if(root->left!=NULL){
		BST_preOrder(root->left, depth);
	}
	if(root->right!=NULL){
		BST_preOrder(root->right, depth);
	}
	depth-=1;
}

void BST_postOrder(BST_Node *root,int depth)
{
	depth+=1;
	if(root->left!=NULL){
		BST_postOrder(root->left, depth);
	}
	if(root->right!=NULL){
		BST_postOrder(root->right, depth);
	}
	depth-=1;
	printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
} 

void delete_BST(BST_Node *root)
{
	if(root!=NULL){
		if(root->left!=NULL){
			delete_BST(root->left);
		}
		if(root->right!=NULL){
			delete_BST(root->right);
		}
		root = BST_delete(root, root->bar, root->index);
	}
}

void freqshift(BST_Node *root, double freq1, double freq2){		//Helper function for BST_shiftFreq.
	if(root->freq==freq1){
		root->freq = freq2;
	}
	if(root->left!=NULL){
		freqshift(root->left, freq1, freq2);
	}
	if(root->right!=NULL){
		freqshift(root->right, freq1, freq2);
	}
}

void BST_shiftFreq(BST_Node *root, char note_src[5], char note_dst[5])
{
	int i=0;
	int k=0;
	while(strcmp(note_names[i],note_src)!=0 && i<100){
		i++;
	}
	while(strcmp(note_names[k],note_dst)!=0 && k<100){
		k++;
	}
	if(i<100 && k<100){
		freqshift(root, note_freq[i], note_freq[k]);
	}
}

double freq_checker(BST_Node *root, int semitones){
	int i=0;
	while(note_freq[i]!=root->freq && i<100){
		i++;
	}
	if(i<100 && i+semitones<100 && i+semitones >= 0){
		return note_freq[i+semitones];
	}
	else{
		return -1.0;
	}
}

BST_Node *BST_harmonize(BST_Node *root, int semitones, double time_shift)
{
	if(root==NULL){
		return NULL;
	}
	if(root->left!=NULL){
		root->left = BST_harmonize(root->left, semitones, time_shift);
	}
	if(root->right!=NULL){
		root->right = BST_harmonize(root->right, semitones, time_shift);
	}
	if(root->index + time_shift > 0 && root->index + time_shift < 1){
		if(freq_checker(root, semitones) > -1.0){
			BST_Node *new_node = newBST_Node(freq_checker(root, semitones), root->bar, root->index + time_shift);
			root = BST_insert(root, new_node);
		}
	}
	return root;
}
