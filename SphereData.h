#pragma once

#include <vector>
#include <QObject>

struct SSphereElement
{
	float x,y,z,r,screenZ;
//	unsigned int dwARGB;
    float red, green, blue;
};

class CFrameBuffer;

class CSphereData : public QObject
{
    Q_OBJECT
public:
    CSphereData( const char *szFilename, CFrameBuffer* fb );
	~CSphereData();

	std::vector<SSphereElement> m_SphereData;
    CFrameBuffer* fb;
    float m_wi;
    float m_fAnimateAngleRatio;
    float m_renderedAngle;

    std::vector<unsigned int> m_times;

public slots:
    void Render();

signals:
    void finishedRendering();
};
