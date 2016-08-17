#include "EasyGeometry.h"
#include <fstream>

EasyGeometry::EasyGeometry(){
  qDebug() << "EasyGeometry created";

  vertices = NULL;

  indices = NULL;
  normals = NULL;
  mappings = NULL;

  vao = NULL;

  vboVertices = NULL;
  vboIndices = NULL;
  vboNormals = NULL;
  vboMappings = NULL;

  scale = 1;
}

void EasyGeometry::bind(){
  // qDebug() << "EasyGeometry:bind";
  vao->bind();
  if(vboMappings){
      vboMappings->bind();
  }
}

void EasyGeometry::draw(){
  // qDebug() << "EasyGeometry:draw";
  vboIndices->bind();
  glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
  vboIndices->release();
}

void EasyGeometry::release(){
  // qDebug() << "EasyGeometry:release";
    if(vboMappings){
        vboMappings->release();
    }
  vao->release();
}

void EasyGeometry::createVBOs() {
    qDebug() << " > createVBOs";

    destroyVBOs();

    vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    vboVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertices->create();
    vboVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertices->bind();
    vboVertices->allocate(vertices, numVertices*sizeof(QVector4D));
    vboVertices->release();
    delete [] vertices;
    vertices = NULL;

    vboNormals = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboNormals->create();
    vboNormals->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboNormals->bind();
    vboNormals->allocate(normals, numVertices*sizeof(QVector3D));
    vboNormals->release();
    delete [] normals;
    normals = NULL;

    vboIndices = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vboIndices->create();
    vboIndices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboIndices->bind();
    vboIndices->allocate(indices,numFaces*3*sizeof(unsigned int));
    vboIndices->release();
    delete [] indices;
    indices = NULL;

    if(mappings){
        // Transfer textures mappings
        vboMappings = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        vboMappings->create();
        vboMappings->bind();
        vboMappings->setUsagePattern(QOpenGLBuffer::StaticDraw);
        vboMappings->allocate(mappings, numVertices * sizeof(QVector2D));
        vboMappings->release();
    }

    vao->release();
    qDebug() << " > vbos created";
}


void EasyGeometry::destroyVBOs(){
    if(vboVertices){
        vboVertices->release();
        vboVertices->destroy();
        delete vboVertices;
        vboVertices = NULL;

    }

    if (vboIndices){
        vboIndices->release();
        vboIndices->destroy();
        delete vboIndices;
        vboIndices = NULL;
    }

    if (vboNormals){
        vboNormals->release();
        vboNormals->destroy();
        delete vboNormals;
        vboNormals = NULL;
    }

    if (vao){
        delete vao;
        vao = NULL;
    }
}

void EasyGeometry::loadFromSTL(QString const & fileName, float normalizeScale) {
    qDebug() << " > Loading STL file: " << fileName;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    //if (file.read(5) == "solid") {
    //    qDebug() << "Accepts only binary STL file!";
    //    return;
    //}

    // Skip the rest of the header material
    file.read(80);

    QDataStream data(&file);
    data.setByteOrder(QDataStream::LittleEndian);
    data.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // Load the triangle count from the .stl file
    // uint32_t tri_count;
    data >> numFaces;
    numVertices = numFaces * 3;

    qDebug() << " > Faces: " << numFaces;
    qDebug() << " > Vertices: " << numVertices;

    // Verify that the file is the right size
    if (file.size() != 84 + numFaces * 50) {
        qDebug() << "Bad STL File";
        return;
    }

    delete [] vertices;
    vertices = new QVector4D [numVertices];

    delete [] indices;
    indices = new unsigned int [numFaces*3];

    delete [] normals;
    normals = new QVector3D [numVertices];


    // Extract vertices into an array of xyz, unsigned pairs
//    QVector<Vec3i> verts(tri_count*3);

    // Dummy array, because readRawData is faster than skipRawData
    char buffer[sizeof(float)*3];

    // Store vertices in the array, processing one triangle at a time.
    double minLim = std::numeric_limits<double>::min();
    double maxLim = std::numeric_limits<double>::max();
    QVector4D max(minLim , minLim , minLim ,1.0);
    QVector4D min(maxLim , maxLim , maxLim ,1.0);
    qDebug() << " > Parse faces";
    float x, y, z;
    for (long i = 0; i < numVertices; i += 3) {
        // Skip face's normal vector
        data.readRawData(buffer, 3*sizeof(float));

        // Load vertex data from .stl file into vertices
        for(int v = 0; v < 3; v++){
            data >> x >> y >> z;
            max.setX(qMax<double>(max.x(),x));
            max.setY(qMax<double>(max.y(),y));
            max.setZ(qMax<double>(max.z(),z));
            min.setX(qMin<double>(min.x(),x));
            min.setY(qMin<double>(min.y(),y));
            min.setZ(qMin<double>(min.z(),z));
            // qDebug() << x << y << z;
            vertices[i + v] = QVector4D(x, y, z, 1.0);
        }

        // Skip face attribute
        data.readRawData(buffer, sizeof(uint16_t));
    }

//    this->offset = -QVector3D((min+max)*0.5) ;

    if(normalizeScale <= 0)
        this->scale = 1/(max-min).length();
    else
        this->scale = normalizeScale;

    qDebug() << " > Min: " << min.x() << min.y() << min.z();
    qDebug() << " > Max: " << max.x() << max.y() << max.z();

    // Save indicies as the second element in the array
    // (so that we can reconstruct triangle order after sorting)
    qDebug() << " > Process indices";
    for (int i = 0; i < numFaces * 3; i++) {
        indices[i] = i;
    }

    // Compute normals of faces
    qDebug() << " > Compute Normals";
    for(unsigned int i = 0; i < numFaces; i++){
        QVector4D a = vertices[i * 3 + 0];
        QVector4D b = vertices[i * 3 + 1];
        QVector4D c = vertices[i * 3 + 2];

        QVector3D n = QVector3D::crossProduct((b - a).toVector3D(), (c - b).toVector3D());
        n.normalize();

        // Create Object3D
        normals[i * 3 + 0] = n;
        normals[i * 3 + 1] = n;
        normals[i * 3 + 2] = n;
    }

    // Normalize normals
    qDebug() << " > Normalize normals";
    for(unsigned int i = 0; i < numVertices; i++){
      normals[i].normalize();
    }

    file.close();
    createVBOs();
}

void EasyGeometry::loadFromOFF(QString const & fileName) {
    QFile file(fileName);
    QTextStream stream(&file);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Failed to open file.");
        return;
    }

    qDebug() << " > Loading OFF file: " << fileName;
    QString line;

    stream>>line;
    stream>>numVertices>>numFaces>>line;

    delete [] vertices;
    vertices = new QVector4D [numVertices];

    delete [] indices ;
    indices = new unsigned int [numFaces*3];

    delete [] normals;
    normals = new QVector3D [numVertices];

    qDebug() << " > Faces: " << numFaces;
    qDebug() << " > Vertices: " << numVertices;

    if (numVertices>0){
        double minLim = std::numeric_limits<double>::min();
        double maxLim = std::numeric_limits<double>::max();
        QVector4D max(minLim , minLim , minLim ,1.0);
        QVector4D min(maxLim , maxLim , maxLim ,1.0);
        for (unsigned int i = 0; i<numVertices ; i++){
            double x, y, z;
            stream>>x>>y>>z;
            max.setX(qMax<double>(max.x(),x));
            max.setY(qMax<double>(max.y(),y));
            max.setZ(qMax<double>(max.z(),z));
            min.setX(qMin<double>(min.x(),x));
            min.setY(qMin<double>(min.y(),y));
            min.setZ(qMin<double>(min.z(),z));
            vertices[i] = QVector4D(x, y, z, 1.0);
        }

        this->offset = QVector3D((min+max)*0.5) ;
        this->scale = 1 / (max-min).length();
    }
    for (unsigned int i = 0; i<numFaces; i++){
        unsigned int a, b, c;
        stream>>line>>a>>b>>c;
        indices[i*3] = a;
        indices[i*3+1] = b;
        indices[i*3+2] = c;
    }

    // Compute normals of faces
    qDebug() << " > Compute Normals";
    for(unsigned int i = 0; i < numFaces; i++){
        QVector4D a = vertices[indices[i * 3 + 0]];
        QVector4D b = vertices[indices[i * 3 + 1]];
        QVector4D c = vertices[indices[i * 3 + 2]];

        QVector3D n = QVector3D::crossProduct((b - a).toVector3D(), (c - b).toVector3D());
        n.normalize();

        // Create Object3D
        normals[indices[i * 3 + 0]] += n;
        normals[indices[i * 3 + 1]] += n;
        normals[indices[i * 3 + 2]] += n;
    }

    // Normalize normals
    qDebug() << " > Normalize normals";
    for(unsigned int i = 0; i < numVertices; i++){
      normals[i].normalize();
    }

    file.close();
    // stream.close();
    createVBOs();
}


