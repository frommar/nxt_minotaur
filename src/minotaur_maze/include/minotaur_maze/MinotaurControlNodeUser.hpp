#ifndef MINOTAUR_MINOTAUR_CONTROL_NODE_USER_HPP
#define MINOTAUR_MINOTAUR_CONTROL_NODE_USER_HPP

#include "minotaur_common/MinotaurControlNode.hpp"
#include "minotaur_common/SensorSettings.hpp"

namespace minotaur
{
     /**
     * \brief The MinotaurControlNodeUser class provides a MinotaurControlNode member
     *        and corresponding setter methods.
     * 
     * Its purpose is to prevent code duplication.
     */
    class MinotaurControlNodeUser
    {
    protected:
        MinotaurControlNode *controlNode;
        std::vector<SensorSetting> sensorSettings;
    public:
        MinotaurControlNodeUser():controlNode(NULL), sensorSettings() { }
        virtual ~MinotaurControlNodeUser() { }
        
        void setMinotaurControlNode(MinotaurControlNode *p_controlNode)
        { controlNode = p_controlNode; sensorSettings = loadCurrentSensorSettings(); }
    };
}

#endif
