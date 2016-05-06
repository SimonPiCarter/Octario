#include "Node.h"

Node::Node() : Movable()
{
	//ctor
}

Node::~Node()
{
	//dtor
}

void Node::addSubNode(std::string name, Node *subNode) {
	subNodes[name] = subNode;
}
Node* Node::getSubNode(std::string name) {
	return subNodes[name];
}

void Node::addDrawable(std::string name, Drawable* drawable) {
	drawables.insert(std::pair<std::string,Drawable*>(name,drawable));
}
Drawable* Node::getDrawable(std::string name) {
	return drawables.at(name);
}

Node* Node::detachSubNode(std::string name) {
	Node* node = subNodes[name];
	subNodes.erase(name);
	return node;
}
Drawable* Node::detachDrawable(std::string name) {
	Drawable* drawable = drawables.at(name);
	drawables.erase(name);
	return drawable;
}

void Node::draw(glm::mat4 modelview, glm::mat4 projection) {
	// Draw all drawables
	for ( std::map<std::string,Drawable*>::iterator it = drawables.begin(); it != drawables.end() ; ++it ) {
		it->second->draw(modelview*transformation,projection);
	}

	// Draw all sub nodes
	for ( std::map<std::string,Node*>::iterator it = subNodes.begin(); it!=subNodes.end(); ++it ) {
		it->second->draw(modelview*transformation,projection);
	}
}
