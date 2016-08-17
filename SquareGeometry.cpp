#include "SquareGeometry.h"

SquareGeometry::SquareGeometry() : EasyGeometry()
{
    qDebug() << " > Generating square...";
    numFaces = 2;
    numVertices = 4;

    delete [] vertices;
    vertices = new QVector4D [numVertices];
    vertices[0] = QVector4D( -1, -1, 0, 1);
    vertices[1] = QVector4D(  1, -1, 0, 1);
    vertices[2] = QVector4D(  1,  1, 0, 1);
    vertices[3] = QVector4D( -1,  1, 0, 1);

    delete [] indices ;
    indices = new unsigned int [numFaces*3];
    indices[0]=0; indices[1]=1; indices[2]=2;
    indices[3]=2; indices[4]=3; indices[5]=0;

    delete [] normals;
    normals = new QVector3D [numVertices];

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

    // Texture Mappings
    mappings = new QVector2D[numVertices];
    mappings[3] = QVector2D(0,0);
    mappings[2] = QVector2D(1,0);
    mappings[1] = QVector2D(1,1);
    mappings[0] = QVector2D(0,1);

    // Normalize normals
    qDebug() << " > Normalize normals";
    for(unsigned int i = 0; i < numVertices; i++){
      normals[i].normalize();
    }

    createVBOs();
}
