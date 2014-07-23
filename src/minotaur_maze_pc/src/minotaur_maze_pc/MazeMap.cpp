#include <stdexcept>
#include <sstream>
#include "minotaur_maze_pc/MazeMap.hpp"

namespace minotaur
{
    MazeMap::MazeMap(unsigned int p_width, unsigned int p_height)
    :width(p_width), height(p_height)
    {
        mazeGrid = new MazeNode*[width];
        for(int i = 0; i < width; ++i)
            mazeGrid[i] = new MazeNode[height];
    }
   
    MazeMap::~MazeMap()
    {
        for(int i = 0; i < width; ++i)
            delete[] mazeGrid[i];
        delete[] mazeGrid;
    }

    MazeNode& MazeMap::node(unsigned int p_x, unsigned int p_y)
    {
        if(p_x >= width) {
            std::stringstream ss;
            ss << "X is greater than width (" << p_x << ")";
            throw std::logic_error(ss.str());
        }
        if(p_y >= height) {
            std::stringstream ss;
            ss << "Y is greater than height (" << p_y << ")";
            throw std::logic_error(ss.str());
        }
        
        return mazeGrid[p_x][p_y];
    }

    unsigned int MazeMap::getWidth()
    {
        return width;
    }
    
    unsigned int MazeMap::getHeight()
    {
        return height;
    }

    void MazeMap::setNodeDimension(float p_width, float p_height)
    {
        nodeWidth = p_width;
        nodeHeight = p_height;
    }
    
    float MazeMap::getNodeWidth()
    {
        return nodeWidth;
    }
    
    float MazeMap::getNodeHeight()
    {
        return nodeHeight;
    }
}
