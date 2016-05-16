#include "DrawableModelLoader.h"

#include <iostream>
#include <fstream>

using namespace std;

DrawableModelLoader::DrawableModelLoader(std::string path, std::string fileName) : m_path(path), m_file(fileName) {
    //ctor
}

DrawableModelLoader::~DrawableModelLoader() {
}

void DrawableModelLoader::loadModel() {
    std::string fullPath_l = m_file;
    if ( m_path.size() != 0 ) {
        fullPath_l = m_path+"/"+m_file;
    }

    std::ifstream file_l;
    file_l.open(fullPath_l.c_str());

    //  File is loaded in raw data
    while ( !file_l.eof() ) {
        std::string buffer_l;
        std::getline(file_l,buffer_l,'\n');
        std::istringstream line_l(buffer_l);
        readLine(line_l);
    }

    file_l.close();


    // Build clean data
    m_vertices = new float[m_rawIbos.size()];
    m_normals = new float[m_rawIbos.size()];
    m_textCoords = new float[m_rawIbos.size()/3*2];

    size_t index_l = 0;
    size_t texIndex_l = 0;
    size_t rawIndex_l = 0;
    while ( rawIndex_l < m_rawIbos.size() ) {
        fillFromIbo(m_vertices,m_rawVertices, index_l, m_rawIbos[rawIndex_l++]-1);
        fillFromIbo2(m_textCoords,m_rawTextCoords, texIndex_l, m_rawIbos[rawIndex_l++]-1);
        fillFromIbo(m_normals,m_rawNormals, index_l, m_rawIbos[rawIndex_l++]-1);

        index_l += 3;
        texIndex_l += 2;
    }
}

void DrawableModelLoader::readLine(std::istringstream& line_p) {
    std::string buffer_l;
    line_p>>buffer_l;
    if ( buffer_l.find("#") != buffer_l.npos ) {
        return;
    }
    float bufferNum_l;
    size_t bufferInt_l;
    if ( buffer_l == "v" ) {
        line_p>>bufferNum_l;        m_rawVertices.push_back(bufferNum_l);
        line_p>>bufferNum_l;        m_rawVertices.push_back(bufferNum_l);
        line_p>>bufferNum_l;        m_rawVertices.push_back(bufferNum_l);
    } else if ( buffer_l == "vn" ) {
        line_p>>bufferNum_l;        m_rawNormals.push_back(bufferNum_l);
        line_p>>bufferNum_l;        m_rawNormals.push_back(bufferNum_l);
        line_p>>bufferNum_l;        m_rawNormals.push_back(bufferNum_l);
    } else if ( buffer_l == "vt" ) {
        line_p>>bufferNum_l;        m_rawTextCoords.push_back(bufferNum_l);
        line_p>>bufferNum_l;        m_rawTextCoords.push_back(bufferNum_l);
    } else if ( buffer_l == "f" ) {
        for ( size_t i = 0 ; i < 9 ; ++ i ) {
            line_p.get();
            line_p>>bufferInt_l;
            m_rawIbos.push_back(bufferInt_l);
        }
    } else if ( buffer_l == "mtllib" ) {
        line_p>>buffer_l;
        readMtl(buffer_l);
    }
}

void DrawableModelLoader::readMtl(std::string path_p) {
    std::string fullPath_l = path_p;
    if ( m_path.size() != 0 ) {
        fullPath_l = m_path+"/"+path_p;
    }
    std::ifstream file_l(fullPath_l.c_str(), std::ifstream::in);

    while ( !file_l.eof() ) {
        std::string buffer_l;
        std::getline(file_l,buffer_l,'\n');
        std::istringstream line_l(buffer_l);

        line_l >> buffer_l;
        if ( buffer_l == "map_Kd" ) {
            line_l >> m_colorTex;
            if ( m_path.size() > 0 ) {
                m_colorTex = m_path + "/"+ m_colorTex;
            }
        } else if ( buffer_l == "disp" ) {
            line_l >> m_bumpTex;
            if ( m_path.size() > 0 ) {
                m_bumpTex = m_path + "/"+ m_bumpTex;
            }
        } else if ( buffer_l == "map_Bump" ) {
            line_l >> m_normalTex;
            if ( m_path.size() > 0 ) {
                m_normalTex = m_path + "/"+ m_normalTex;
            }
        }
    }

    file_l.close();
}

void DrawableModelLoader::fillFromIbo(float* array_p, std::vector<float>& source_p, size_t index_p, size_t indexSource_p) {
    array_p[index_p+0] = source_p[indexSource_p*3+0];
    array_p[index_p+1] = source_p[indexSource_p*3+1];
    array_p[index_p+2] = source_p[indexSource_p*3+2];
}

void DrawableModelLoader::fillFromIbo2(float* array_p, std::vector<float>& source_p, size_t index_p, size_t indexSource_p) {
    array_p[index_p+0] = source_p[indexSource_p*2+0];
    array_p[index_p+1] = source_p[indexSource_p*2+1];
}

float* DrawableModelLoader::getVertices()
{
    return m_vertices;
}

float* DrawableModelLoader::getNormals()
{
    return m_normals;
}

float* DrawableModelLoader::getTexCoords()
{
    return m_textCoords;
}

string DrawableModelLoader::getColorTexture() const
{
    return m_colorTex;
}

string DrawableModelLoader::getNormalTexture() const
{
    return m_normalTex;
}

string DrawableModelLoader::getBumpTexture() const
{
    return m_bumpTex;
}

size_t DrawableModelLoader::getNbVertices() const {
    return m_rawIbos.size()/3;
}


