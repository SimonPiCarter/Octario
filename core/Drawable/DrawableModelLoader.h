#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "../Tools/Singleton.h"
#include "DrawableModel.h"

class DrawableModelLoader
{
    public:
        DrawableModelLoader(std::string path, std::string fileName);
        virtual ~DrawableModelLoader();

        void loadModel();

        float* getVertices();
        float* getNormals();
        float* getTexCoords();

        std::string getColorTexture() const;
        std::string getNormalTexture() const;
        std::string getBumpTexture() const;

        size_t getNbVertices() const;

    protected:

    private:
        void readLine(std::istringstream& line_p);

        void readMtl(std::string path_p);

        void fillFromIbo(float* array_p, std::vector<float>& source_p, size_t index_p, size_t indexSource_p);
        void fillFromIbo2(float* array_p, std::vector<float>& source_p, size_t index_p, size_t indexSource_p);

        std::string m_path;
        std::string m_file;

        float* m_vertices;
        float* m_normals;
        float* m_textCoords;

        std::string m_colorTex;
        std::string m_normalTex;
        std::string m_bumpTex;

        std::vector<float> m_rawVertices;
        std::vector<float> m_rawNormals;
        std::vector<float> m_rawTextCoords;
        std::vector<size_t> m_rawIbos;
};
