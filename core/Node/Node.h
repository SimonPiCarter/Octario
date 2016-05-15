#pragma once

#include "../Movable/Movable.h"
#include "../Drawable/Drawable.h"
#include "../Drawable/DebugDrawable.h"

#include <map>
#include <set>
#include <string>

class Node : public Movable
{
	public:
		/** Default constructor */
		Node();
		/** Default destructor */
		virtual ~Node();

		void addSubNode(std::string name, Node* subNode);
		Node* getSubNode(std::string name);

		void addDrawable(std::string name, Drawable* drawable);
		Drawable* getDrawable(std::string name);

		void addDebugDrawable(std::string name, DebugDrawable* drawable);
		DebugDrawable* getDebugDrawable(std::string name);

		Node* detachSubNode(std::string name);
		Drawable* detachDrawable(std::string name);

		void draw(const glm::mat4& view, glm::mat4 modelMat, const glm::mat4& projection, const Shader& shader);
		void drawDebug(const glm::mat4& view, glm::mat4 modelMat, const glm::mat4& projection, const Shader& shader);

	protected:
		std::map<std::string, Node*> subNodes;
		std::map<std::string, Drawable*> drawables;
		std::map<std::string, DebugDrawable*> debugDrawables;
	private:
};

