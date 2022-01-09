#pragma once
#include "WaterObstacle.h"

enum class eLogSegment : UINT8
{
    logMiddle,
    logEndLeft,
    logEndRight
};

class LogSegment final :
    public WaterObstacle
{
public:

    LogSegment();
    virtual ~LogSegment() = default;

    void SetLogSegment(eLogSegment segment);

private:

    class Texture* m_pTexture;

};

