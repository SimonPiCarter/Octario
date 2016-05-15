#include "Node.h"

using namespace glm;

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

void Node::addDebugDrawable(std::string name, DebugDrawable* drawable) {
	debugDrawables.insert(std::pair<std::string,DebugDrawable*>(name,drawable));
}
DebugDrawable* Node::getDebugDrawable(std::string name) {
    return debugDrawables.at(name);
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

void Node::draw(const glm::mat4& view, glm::mat4 modelMat, const glm::mat4& projection, const Shader& shader) {
    modelMat *= transformation;

    mat4 mvp = projection*view*modelMat;

	glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "mvp"), 1, GL_FALSE, value_ptr(mvp));
	glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "model"), 1, GL_FALSE, value_ptr(modelMat));

	// Draw all drawables
	for ( std::map<std::string,Drawable*>::iterator it = drawables.begin(); it != drawables.end() ; ++it ) {
		it->second->draw(shader);
	}

	// Draw all sub nodes
	for ( std::map<std::string,Node*>::iterator it = subNodes.begin(); it!=subNodes.end(); ++it ) {
		it->second->draw(view,modelMat,projection,shader);
	}
}

void Node::drawDebug(const glm::mat4& view, glm::mat4 modelMat, const glm::mat4& projection, const Shader& shader) {
    modelMat *= transformation;

    mat4 mvp = projection*view*modelMat;

	glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "mvp"), 1, GL_FALSE, value_ptr(mvp));

	// Draw all drawables
	for ( std::map<std::string,DebugDrawable*>::iterator it = debugDrawables.begin(); it != debugDrawables.end() ; ++it ) {
		it->second->draw(shader);
	}

	// Draw all sub nodes
	for ( std::map<std::string,Node*>::iterator it = subNodes.begin(); it!=subNodes.end(); ++it ) {
		it->second->draw(view,modelMat,projection,shader);
	}
}
