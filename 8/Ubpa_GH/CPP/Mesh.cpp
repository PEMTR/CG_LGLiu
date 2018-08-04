#include "Mesh.h"

void Mesh::loadRectMesh(Img * img, int n){
	int xMax = img->width - 1, yMax = img->height - 1;
	int iNum = 2 + (xMax-1) / n, jNum = 2 + (yMax-1) / n;

	// get mesh point
	for(int x=0; x<xMax; x+=n){
		for(int y=0; y<yMax; y+=n)
			V.push_back(ND_Point(x, y));
		V.push_back(ND_Point(x, yMax));
	}
	for(int y=0; y<yMax; y+=n)
		V.push_back(ND_Point(xMax,y));
	V.push_back(ND_Point(xMax,yMax));
	
	// get mesh face
	for(int i=0; i<iNum-1; i++){
		for(int j=0; j<jNum-1; j++){
			Face face;
			face.push_back(i*jNum + j);
			face.push_back((i+1)*jNum + j);
			face.push_back((i+1)*jNum + j+1);
			face.push_back(i*jNum + j+1);
			faces.push_back(face);
		}
	}

	oldV = V;
}

void Mesh::loadTriRandMesh(Img * img, int n){
	V = getRandP(img->width, img->height, n);
	oldV = V;
	genTriMesh(V, faces);
}

void Mesh::loadTriFeatureMesh(Img * img){
	
	// ��ӱ߽��
	V.clear();
	int step = 20;
	int width = img->width, height = img->height;
	for (int i = step; i < width; i += step) {
		V.push_back(ND_Point(i, 0));
		V.push_back(ND_Point(i, height - 1));
	}
	for (int i = step; i < height; i += step) {
		V.push_back(ND_Point(0, i));
		V.push_back(ND_Point(width - 1, i));
	}
	V.push_back(ND_Point(0, 0));
	V.push_back(ND_Point(height-1, 0));
	V.push_back(ND_Point(height-1, width-1));
	V.push_back(ND_Point(0, width-1));
	
	// ���������
	std::vector<ND_Point> keypoints;
	FAST_API(*img, keypoints, 30);
	
	for(int i=0; i<keypoints.size(); i++)
		if(img->contains(keypoints[i]))
			V.push_back(keypoints[i]);
	/*
	cv::Mat imgMat;// = ImgToMat(*img);
	cv::SurfFeatureDetector surf;
	surf.detect(imgMat, keypoints);
	
	for(int i=0; i<keypoints.size(); i++)
		//detector �� bug
		if(keypoints[i].pt.x > -1000 && keypoints[i].pt.y > -1000)
			V.push_back(ND_Point(keypoints[i].pt.x, keypoints[i].pt.y));
	*/
	oldV = V;
	
	//V = getRandP(img->width, img->height, 16);
	//oldV = V;
	genTriMesh(V, faces);
}

Img * Mesh::warp(Img * img, Interpolation & interpolation){

	Img * newImg = interpolation.warp(img, V);
	for(int i=0; i<V.size(); i++)
		V[i] += oldV[i];

	for(int i=0; i<faces.size(); i++){
		Face & face = faces[i];
		std::vector<ND_Point> points = polygonScan(face2PVec(face), newImg->height);
		for(int j=0; j<points.size(); j++){
			ND_Point oldP = getOriginalP(points[j], face);
			if(newImg->contains(points[j]) && img->contains(oldP))
				newImg->at(points[j]) = img->at(oldP);
		}
	}
	return newImg;
}

ND_Point Mesh::getOriginalP(ND_Point & p, Face & face){
	if(face.size() == 3)
		return getOriginalP_Tri(p, face);
	else
		return getOriginalP_Rect(p, face);
}

ND_Point Mesh::getOriginalP_Rect(ND_Point & p, Face & face){
	float S2[4], cotAlpha[4], cotBeta[4], w[4], sumW = 0, lambda[4], x = 0, y = 0;
	bool isCorner = false;
	int a2[4], r2[4];

	for(int k=0; k<4; k++){
		a2[k] = (V[face[k]] - V[face[(k+1)%4]]).mod2();
		r2[k] = (p - V[face[k]]).mod2();
		if(r2[k]==0){
			isCorner = true;
			break;
		}
		S2[k] = pow(ND_Point::crossProduct(p-V[face[k]], V[face[(k+1)%4]] - V[face[k]])/ 2.0, 2);
	}
	
	if(isCorner)
		return ND_Point(-1,-1);
	
	for(int k=0; k<4; k++){
		cotAlpha[k] = S2[k]==0 ? sqrt((float)(a2[k]*r2[k])) * 999999.0 : sqrt(a2[k]*r2[k]/S2[k] - 1);
		cotBeta[k] = S2[(k+3)%4]==0 ? sqrt((float)(a2[(k+3)%4] * r2[k])) * 999999.0 : sqrt(a2[(k+3)%4]*r2[k]/S2[(k+3)%4]-1);
		w[k] = (cotAlpha[k] + cotBeta[k]) / r2[k];
		sumW += w[k];
	}

	for(int k=0; k<4; k++)
		lambda[k] = w[k] / sumW;

	for(int k=0; k<4; k++){
		x += oldV[face[k]].x * lambda[k];
		y += oldV[face[k]].y * lambda[k];
	}

	return ND_Point(x, y);
}

ND_Point Mesh::getOriginalP_Tri(ND_Point & p, Face & face){
	float lambda[3];
	float SofFace = ND_Point::SofTri(V[face[0]], V[face[1]], V[face[2]]);

	lambda[0] = ND_Point::SofTri(p, V[face[1]], V[face[2]]) / SofFace;
	lambda[1] = ND_Point::SofTri(V[face[0]], p, V[face[2]]) / SofFace;
	lambda[2] = ND_Point::SofTri(V[face[0]], V[face[1]], p) / SofFace;

	float x = 0, y = 0;
	for(int k=0; k<3; k++){
		x += oldV[face[k]].x * lambda[k];
		y += oldV[face[k]].y * lambda[k];
	}
	int intX = x-floor(x) > 0.5 ? ceil(x) : floor(x);
	int intY = y-floor(y) > 0.5 ? ceil(y) : floor(y);

	return ND_Point(intX, intY);
}

std::vector<ND_Point> Mesh::polygonScan(std::vector<ND_Point> & polygon, int windowHeight){
	std::vector<ND_Point> rst;

	//������ߵ��y����
	int maxY = polygon[0].y;
	int minY = polygon[0].y;
	for(int i=1; i<polygon.size(); i++){
		if(polygon[i].y > maxY)
			maxY = polygon[i].y;
		if(polygon[i].y < minY)
			minY = polygon[i].y;
	}
	int N = 1+maxY-minY;
	//��ʼ��ET��AET
	Edge ** ET = (Edge **)malloc(N*sizeof(Edge*));
	for (int i=0; i<N; i++){
		ET[i] = new Edge();
		ET[i]->next = nullptr;
	}
	Edge * AET = new Edge();
	AET->next = nullptr;
	
	//�����߱�ET
	for(int i=0; i<polygon.size(); i++){
		//ȡ����ǰ��1ǰ�����ڵĹ�4���㣬��1���2��������Ϊ����ѭ������ıߣ������������0�͵�3���ڼ������
		int x0 = polygon[(i - 1 + polygon.size()) % polygon.size()].x;
		int y0 = polygon[(i - 1 + polygon.size()) % polygon.size()].y;
		int x1 = polygon[i].x;
		int y1 = polygon[i].y;
		int x2 = polygon[(i + 1) % polygon.size()].x;
		int y2 = polygon[(i + 1) % polygon.size()].y;
		int x3 = polygon[(i + 2) % polygon.size()].x;
		int y3 = polygon[(i + 2) % polygon.size()].y;
		//ˮƽ��ֱ������
		if(y1 == y2)
			continue;
		//�ֱ�����¶˵�y���ꡢ�϶˵�y���ꡢ�¶˵�x�����б�ʵ���
		int ymin = y1 > y2 ? y2 : y1;
		int ymax = y1 > y2 ? y1 : y2;
		float x = y1 > y2 ? x2 : x1;
		float dx = ((float)(x1 - x2)) / (y1 - y2);
		//������⴦������2->1->0��y���굥���ݼ���y1Ϊ��㣬����1->2->3��y���굥���ݼ���y2Ϊ���
		if((y0<y1 && y1<y2) || (y1>y2 && y2>y3)){
			ymin++;
			x += dx;
		}
		//�����±ߣ�����߱�ET
		int idx = ymin-minY;
		if(idx>=0 && idx<N){
			Edge * p = new Edge();
			p->ymax = ymax;
			p->x = x;
			p->dx = dx;
			p->next = ET[idx]->next;
			ET[idx]->next = p;
		}
	}
	
	//ɨ���ߴ�������ɨ�裬y����ÿ�μ�1
	for(int i=0; i<N; i++){
		//ȡ��ET�е�ǰɨ���е����б߲���x�ĵ���˳����x�����dx�ĵ���˳�򣩲���AET
		while(ET[i]->next){
			//ȡ��ET�е�ǰɨ���б�ͷλ�õı�
			Edge *pInsert = ET[i]->next;
			Edge *p = AET;
			//��AET���������ʵĲ���λ��
			while(p->next){
				if(pInsert->x > p->next->x){
					p = p->next;
					continue;
				}
				if(pInsert->x == p->next->x && pInsert->dx > p->next->dx){
					p = p->next;
					continue;
				}
				//�ҵ�λ��
				break;
			}
			//��pInsert��ET��ɾ����������AET�ĵ�ǰλ��
			ET[i]->next = pInsert->next;
			pInsert->next = p->next;
			p->next = pInsert;
		}
		
		//AET�еı�������Բ���ɫ
		Edge *p = AET;
		while(p->next && p->next->next){
			for(int x = p->next->x; x < p->next->next->x; x++)
				rst.push_back(ND_Point(x, i+minY));
			p = p->next->next;
		}
		
		//ɾ��AET������y=ymax�ı�
		p = AET;
		while(p->next){
			if(p->next->ymax == i+minY){
				Edge * pDelete = p->next;
				p->next = pDelete->next;
				pDelete->next = nullptr;
				delete pDelete;
			}else
				p = p->next;
		}
		
		//����AET�бߵ�xֵ��������һѭ��
		p = AET;
		while(p->next){
			p->next->x += p->next->dx;
			p = p->next;
		}
	}
	
	//�˴���һ�����ͷŸɾ������������ڴ�����
	for(int i=0; i<N; i++)
		delete ET[i];
	free(ET);
	
	return rst;
}

std::vector<ND_Point> Mesh::getRandP(int width, int height, int n){
	std::vector<ND_Point> rst;

	// ��ӱ߽��
	int step = 20;
	for (int i = step; i < width; i += step) {
		rst.push_back(ND_Point(i, 0));
		rst.push_back(ND_Point(i, height - 1));
	}
	for (int i = step; i < height; i += step) {
		rst.push_back(ND_Point(0, i));
		rst.push_back(ND_Point(width - 1, i));
	}
	rst.push_back(ND_Point(0, 0));
	rst.push_back(ND_Point(height-1, 0));
	rst.push_back(ND_Point(height-1, width-1));
	rst.push_back(ND_Point(0, width-1));

	// ����ڲ������
	time_t t;
	srand((unsigned) time(&t));
	for(int i=0; i<n; i++)
		rst.push_back(ND_Point(rand()%(width-2)+1, rand()%(height-2)+1));
	return rst;
}

std::vector<ND_Point> Mesh::face2PVec(Face & face){
	std::vector<ND_Point> rst;
	for(int i=0; i<face.size(); i++)
		rst.push_back(V[face[i]]);
	return rst;
}

/*
std::vector<ND_Point> Mesh::getPInPolygon(std::vector<ND_Point> & polygon){
	if(polygon.size()==0)
		return std::vector<ND_Point>();

	std::vector<ND_Point> rst;
	int xMax, xMin, yMax, yMin;
	xMax = xMin = polygon[0].x;
	yMax = yMin = polygon[0].y;
	
	QPolygon qPolygon;
	for(int i=0; i<polygon.size(); i++){
		qPolygon.putPoints(i, 1, polygon[i].x, polygon[i].y);
		if(polygon[i].x > xMax) xMax = polygon[i].x;
		if(polygon[i].x < xMin) xMin = polygon[i].x;
		if(polygon[i].y > yMax) yMax = polygon[i].y;
		if(polygon[i].y < yMin) yMin = polygon[i].y;
	}

	for(int x=xMin; x<=xMax; x++){
		for(int y=yMin; y<=yMax; y++){
			if(qPolygon.containsPoint(QPoint(x,y), Qt::OddEvenFill))
				rst.push_back(ND_Point(x,y));
		}
	}

	return rst;
}*/