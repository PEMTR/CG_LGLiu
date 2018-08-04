#ifndef MESH_H
#define MESH_H

#include "Interpolation.h"

//�������ڱ߱�ET�ͻ�߱�AET��ͨ����Edge
typedef struct edge{
	int ymax;
	float x;
	float dx;
	struct edge * next;
} Edge;

typedef std::vector<int> Face;

class Mesh : public Interpolation{
public:
	// generate mesh
	void loadRectMesh(QImage * img, int n);
	void loadTriRandMesh(QImage * img, int n);
	void loadTriFeatureMesh(QImage * img);

	std::vector<ND_Point> face2PVec(Face & face);

	QImage * warp(QImage * img, Interpolation & interpolation);
	std::vector<ND_Point> polygonScan(std::vector<ND_Point> & polygon, int windowHeight);

private:
	//�򵥵�ʹ�� contains ���жϵ��Ƿ���polygon��
	//std::vector<ND_Point> getPInPolygon(std::vector<ND_Point> & polygon);
	ND_Point getOriginalP(ND_Point & p, Face & face);
	ND_Point getOriginalP_Rect(ND_Point & p, Face & face);
	ND_Point getOriginalP_Tri(ND_Point & p, Face & face);

	std::vector<ND_Point> getRandP(int width, int height, int n);
	void genTriMesh();
	void genTriMesh(std::vector<ND_Point> & V, std::vector<std::vector<int>> & faces);

	std::vector<ND_Point> V, oldV;
	std::vector<Face> faces;
};

#endif // MESH_H