#include "BranchMesh.h"

void BranchMesh::setup(ofVec3f sPoint, ofVec3f ePoint, int res, float sRadius, float eRadius)
{
    ofNode nodeS, nodeE;
    ofVec3f vec = ePoint - sPoint;
    ofQuaternion q;
    q.makeRotate(ofVec3f(0, 0, 1), vec);
    
    nodeS.move(sPoint.x, sPoint.y, sPoint.z);
    nodeS.setOrientation(q);
    ofMatrix4x4 matrixS = nodeS.getGlobalTransformMatrix();
    
    nodeE.move(sPoint.x, sPoint.y, sPoint.z);
    nodeE.setOrientation(q);
    nodeE.move(vec);
    ofMatrix4x4 matrixE = nodeE.getGlobalTransformMatrix();
    
    // the x & y resolution for the sphere is double the input res number so doubling the cylinder res to match with the sphere
    setupCylinder(2 * res, sRadius, eRadius, vec.length(), matrixS);
    setupSphere(res, eRadius, matrixE);
}

void BranchMesh::draw()
{
    cylinderMesh.draw();
    sphereMesh.draw();
}

void BranchMesh::setupCylinder(int res, float sRadius, float eRadius, float z, ofMatrix4x4 m)
{
    float angle = TWO_PI / res;
    for (int i = 0; i < res; i++)
    {
        float x = cos(i * angle) * sRadius;
        float y = sin(i * angle) * sRadius;
        cylinderMesh.addVertex(ofPoint(x, y, 0) * m);
        cylinderMesh.addColor(ofFloatColor(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0)));
    }
    
    for (int i = 0; i < res; i++)
    {
        float x = cos(i * angle) * eRadius;
        float y = sin(i * angle) * eRadius;
        cylinderMesh.addVertex(ofPoint(x, y, z) * m);
        cylinderMesh.addColor(ofFloatColor(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0)));
    }
    
    cylinderMesh.addVertex(ofPoint(0, 0, 0) * m);
    cylinderMesh.addColor(ofFloatColor(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0)));
    cylinderMesh.addVertex(ofPoint(0, 0, z) * m);
    cylinderMesh.addColor(ofFloatColor(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0)));
    
    for (int i = 0; i < res; i++)
    {
        // cylinder body
        cylinderMesh.addIndex(i);
        cylinderMesh.addIndex((i == res - 1) ? 0 : i + 1);
        cylinderMesh.addIndex((i == res - 1) ? res : i + 1 + res);

        cylinderMesh.addIndex(i);
        cylinderMesh.addIndex((i == res - 1) ? res : i + 1 + res);
        cylinderMesh.addIndex(i + res);
        
        // cylinder caps
        cylinderMesh.addIndex(i);
        cylinderMesh.addIndex(2 * res);
        cylinderMesh.addIndex((i == res - 1) ? 0 : i + 1);
        
        cylinderMesh.addIndex(i + res);
        cylinderMesh.addIndex((i == res - 1) ? res : i + 1 + res);
        cylinderMesh.addIndex(2 * res + 1);
    }
}

void BranchMesh::setupSphere(int res, float radius, ofMatrix4x4 m)
{
    float phi = TWO_PI / (2 * res);
    float theta = PI / res;
    for (int i = 1; i < res; i++)
        for (int j = 0; j < 2 * res; j++)
        {
            float x = radius * sin(i * theta) * cos(j * phi);
            float y = radius * sin(i * theta) * sin(j * phi);
            float z = radius * cos(i * theta);
            sphereMesh.addVertex(ofPoint(x, y, z) * m);
            sphereMesh.addColor(ofFloatColor(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0)));
        }
    
    for (int i = 0; i <= PI; i += PI)
    {
        float x = 0;
        float y = 0;
        float z = radius * cos(i);
        sphereMesh.addVertex(ofPoint(x, y, z) * m);
        sphereMesh.addColor(ofFloatColor(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0)));
    }
    
    for (int i = 0; i < 2 * res; i++)
        for (int j = 0; j < res; j++)
        {
            if (j == 0)
            {
                sphereMesh.addIndex(2 * res * (res - 1));
                sphereMesh.addIndex((i == 2 * res - 1) ? (j * 2 * res) +       (0) : (j * 2 * res)       + (i + 1));
                sphereMesh.addIndex(                                                 (j * 2 * res)       + (i));
            }
            else if (j < res - 1)
            {
                int jj = j - 1;
                sphereMesh.addIndex(                                                  (jj * 2 * res)       + (i));
                sphereMesh.addIndex((i == 2 * res - 1) ? (jj * 2 * res) +       (0) : (jj * 2 * res)       + (i + 1));
                sphereMesh.addIndex((i == 2 * res - 1) ? ((jj + 1) * 2 * res) + (0) : ((jj + 1) * 2 * res) + (i + 1));
                
                sphereMesh.addIndex(                                                  (jj * 2 * res)       + (i));
                sphereMesh.addIndex((i == 2 * res - 1) ? ((jj + 1) * 2 * res) + (0) : ((jj + 1) * 2 * res) + (i + 1));
                sphereMesh.addIndex(                                                  ((jj + 1) * 2 * res) + (i));
            }
            else if (j == res - 1)
            {
                int jj = j - 1;
                sphereMesh.addIndex(                                                  (jj * 2 * res)       + (i));
                sphereMesh.addIndex((i == 2 * res - 1) ? (jj * 2 * res) +       (0) : (jj * 2 * res)       + (i + 1));
                sphereMesh.addIndex(2 * res * (res - 1) + 1);
            }
        }
}
