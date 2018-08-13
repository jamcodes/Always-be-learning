#include <iostream>
#include <vector>
#include <memory>
#include <cassert>


struct TreeNode
{
    enum Kind{RED, BLUE};

    TreeNode(Kind kind_) : kind(kind_) { }
    TreeNode(Kind kind_, TreeNode* left_, TreeNode* right_)
        : kind(kind_), left(left_), right(right_)
        { }

    Kind kind;
    TreeNode* left{nullptr};
    TreeNode* right{nullptr};
};

template<typename Derived>
class GenericVisitor
{
public:
    void visit_preorder(TreeNode* node)
    {
        if(node){
            dispatch_node(node);
            visit_preorder(node->left);
            visit_preorder(node->right);
        }
    }

    void visit_inorder(TreeNode* node)
    {
        if(node){
            visit_inorder(node->left);
            dispatch_node(node);
            visit_inorder(node->right);
        }
    }

    void visit_postorder(TreeNode* node)
    {
        if(node){
            visit_postorder(node->left);
            visit_postorder(node->right);
            dispatch_node(node);
        }
    }
protected:
    void handle_RED(TreeNode* node)
    {
        std::cout << "Generic handle RED " << node <<"\n";
    }
    
    void handle_BLUE(TreeNode* node)
    {
        std::cout << "Generic handle BLUE " << node << "\n";
    }
private:
    // convenience method for CRTP
    Derived& derived()
    {
        return static_cast<Derived&>(*this);
    }
    const Derived& derived() const
    {
        return static_cast<const Derived&>(*this);
    }

    void dispatch_node(TreeNode* node)
    {
        switch(node->kind){
        case TreeNode::RED:
            derived().handle_RED(node);
            break;
        case TreeNode::BLUE:
            derived().handle_BLUE(node);
            break;
        default:
            assert(false);
        }
    }
};

class DefaultVisitor : public GenericVisitor<DefaultVisitor>
{
};

class SpecialVisitor : public GenericVisitor<SpecialVisitor>
{
protected:
// need to make the base class a friend because handle_RED is protected
// or just make it public...
    friend class GenericVisitor<SpecialVisitor>;
    void handle_RED(TreeNode* node)
    {
        std::cout << "RED special visitor " << node << "\n";
    }
};

class BlueVisitor : public GenericVisitor<BlueVisitor>
{
protected:
    friend class GenericVisitor<BlueVisitor>;
    void handle_BLUE(TreeNode* node)
    {
        std::cout << "BLUE dedicated visitor " << node << "\n";
    }
};

int main()
{

    auto tn1 = TreeNode(TreeNode::BLUE);
    auto tn2 = TreeNode(TreeNode::RED);
    auto tn3 = TreeNode(TreeNode::BLUE, &tn1, &tn2);

    auto default_visitor = DefaultVisitor{};
    auto special_visitor = SpecialVisitor{};
    auto blue_visitor = BlueVisitor{};

    default_visitor.visit_preorder(&tn3);
    default_visitor.visit_inorder(&tn3);
    default_visitor.visit_postorder(&tn3);

    special_visitor.visit_preorder(&tn3);
    special_visitor.visit_inorder(&tn3);
    special_visitor.visit_postorder(&tn3);

    blue_visitor.visit_preorder(&tn3);
    blue_visitor.visit_inorder(&tn3);
    blue_visitor.visit_postorder(&tn3);
}
