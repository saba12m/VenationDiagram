#include "Venation3D.h"

void Venation3D::setup(int _noOfAttractors, int _nodeRadius, float _degreeOfOpenness)
{
    openAttractors.clear();
    closedAttractors.clear();
    tris.clear();
    //    // geometry
    //    ofxAssimpModelLoader geometry;
    //    ofMesh geometryMesh;
    //    ofxAssimpModelLoader surface;
    //    ofMesh surfaceMesh;
    //
    //    ofBoxPrimitive boundingBox; //
    
    geometry.loadModel("Test 1 - Geometry.obj");
    geometryMesh = geometry.getMesh(0);
    
    surface.loadModel("Test 1 - Surface.obj");
    surfaceMesh = surface.getMesh(0);
    
    //    vector <ofVec3f> points;
    
    // get bounding box
    float minX =   std::numeric_limits<float>::max();
    float maxX = - std::numeric_limits<float>::max();
    float minY =   std::numeric_limits<float>::max();
    float maxY = - std::numeric_limits<float>::max();
    float minZ =   std::numeric_limits<float>::max();
    float maxZ = - std::numeric_limits<float>::max();
    
    auto verts = geometryMesh.getVertices();
    for (int i = 0; i < verts.size(); i++)
    {
        auto v = verts[i];
        if(v.x < minX) minX = v.x;
        if(v.x > maxX) maxX = v.x;
        if(v.y < minY) minY = v.y;
        if(v.y > maxY) maxY = v.y;
        if(v.z < minZ) minZ = v.z;
        if(v.z > maxZ) maxZ = v.z;
    }
    
    boundingBox.set(maxX - minX, maxY - minY, maxZ - minZ); //
    boundingBox.setPosition((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2); //
    
    //    // raycasting
    //    ofxRayTriangleIntersection rtIntersect;
    //    vector <FaceTri> tris;
    //    vector <Ray> rays;
    //    vector <ofVec3f> pIn;
    //    vector <ofVec3f> pOut;
    
    // input mesh faces
    for (int i = 0; i < geometryMesh.getUniqueFaces().size(); i++)
    {
        auto face = geometryMesh.getFace(i);
        
        FaceTri tri;
        auto v = face.getVertex(0);
        tri.v0 = ofPoint(v.x, v.y, v.z);
        v = face.getVertex(1);
        tri.v1 = ofPoint(v.x, v.y, v.z);
        v = face.getVertex(2);
        tri.v2 = ofPoint(v.x, v.y, v.z);
        tris.push_back(tri);
    }
    
    // surface population
    float ratioForSurface = 0.4;
    int num = _noOfAttractors * ratioForSurface;
    float totalArea = 0;
    vector <float> areas;
    areas.clear();
    areas.resize(surfaceMesh.getUniqueFaces().size());
    
    // 1. area calculation
    for (int i = 0; i < areas.size(); i++)
    {
        auto face = surfaceMesh.getFace(i);
        
        auto a = face.getVertex(0);
        auto b = face.getVertex(1);
        auto c = face.getVertex(2);
        
        ofVec3f ab = b - a;
        ofVec3f bc = c - b;
        float cAngle = ab.angleRad(bc);
        
        float area = 0.5 * ab.length() * bc.length() * sin(cAngle);
        totalArea += area;
        areas[i] = area;
    }
    
    // 2. point distribution
    for (int i = 0; i < areas.size(); i++)
    {
        int triNum = round(num * areas[i] / totalArea);
        
        auto face = surfaceMesh.getFace(i);
        auto a = face.getVertex(0);
        auto b = face.getVertex(1);
        auto c = face.getVertex(2);
        ofVec3f ba = a - b;
        ofVec3f bc = c - b;
        
        for (int j = 0; j < triNum; j++)
        {
            float baVar = ofRandom(1.0);
            float bcVar = ofRandom(1.0);
            
            // if they go outside the triangle and into the quadrilateral
            if (baVar + bcVar > 1.0)
            {
                float baTemp = baVar;
                baVar = 1 - bcVar;
                bcVar = 1 - baTemp;
            }
            ofVec3f p = baVar * ba + bcVar * bc;
            p += b;
            if (ofRandom(1.0) < _degreeOfOpenness) openAttractors.push_back(p);
            else closedAttractors.push_back(p);
        }
    }
    
    // volume population
    while (openAttractors.size() + closedAttractors.size() < _noOfAttractors)
//    for (int i = 0; i < n; i ++)
    {
        rays.clear();
        float x = ofRandom(minX, maxX);
        float y = ofRandom(minY, maxY);
        float z = ofRandom(minZ, maxZ);
        ofVec3f dir = ofVec3f(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0));
        dir.normalize();
        dir *= (maxX - minX) + (maxY - minY) + (maxZ - minZ);
        Ray ray;
        ray.rayOrig.set(x, y, z);
        ray.rayEnd.set(x + dir.x, y + dir.y, z + dir.z);
        rays.push_back(ray);
        auto results = rtIntersect.checkMeshIntersection(rays, tris);
        if (results.size() % 2 == 1)
        {
            if (ofRandom(1.0) < _degreeOfOpenness) openAttractors.push_back(ofVec3f(x, y, z));
            else closedAttractors.push_back(ofVec3f(x, y, z));
        }
    }
    
    cout << openAttractors.size() << " - " << closedAttractors.size() << endl;
}

void Venation3D::update()
{
    
}

void Venation3D::draw()
{
    // bounding box
    ofSetColor(0, 0, 255);
    boundingBox.drawWireframe();
    
    // geometry mesh
    ofSetColor(0, 255, 0);
    geometryMesh.drawWireframe();
    
    // surface mesh
    ofSetColor(255, 255, 0);
    surfaceMesh.drawWireframe();
    
    // drawing points
    ofPushStyle();
    ofFill();
    
    ofSetColor(0, 255, 255);
    for (int i = 0; i < openAttractors.size(); i++)
        ofDrawSphere(openAttractors[i].x, openAttractors[i].y, openAttractors[i].z, 0.2);
    
    ofSetColor(255, 0, 0);
    for (int i = 0; i < closedAttractors.size(); i++)
        ofDrawSphere(closedAttractors[i].x, closedAttractors[i].y, closedAttractors[i].z, 0.2);
    
    ofPopStyle();
    
}
