//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include <iostream>
#include <random>
#include <chrono>

#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

std::string ExePath = "../";

class CClusterLayer : public CKhuGleLayer {
public:
	std::vector<CKhuGleSprite *> m_Point;
	double radiusOfNeighborhood;
	int minimumOfNeighborhood;
	std::vector<std::string> m_Head;
	std::vector<std::vector<double>> m_ReadData;
	std::string path = "dartboard1.csv";

	CClusterLayer(int nW, int nH, KgColor24 bgColor, CKgPoint ptPos = CKgPoint(0, 0))
		: CKhuGleLayer(nW, nH, bgColor, ptPos) {

		ReadData();
		Normalization();
		GenerateData();

		// �ݰ� 12 �� �̿��� ���� ������ 5�� �̻��̶�� �߽���
		radiusOfNeighborhood = 12;
		minimumOfNeighborhood = 5;
	}
	void GenerateData();
	void GenerateRandomData(int nCnt);
	void ReadData();
	void Normalization();
	void ChangeDataset();
};

// NŰ�� �����ٸ�
// csv ������ �ҷ���
void CClusterLayer::ReadData() {
	std::string CsvPath;
	CsvPath = ExePath + path;
	ReadCsv(CsvPath, m_Head, m_ReadData);
}

// csv ������ ���
// �� �����͵��� [0, 1]�� ����ȭ
void CClusterLayer::Normalization() {
	for (int index = 0; index < 2; index++) {
		double max = -1e16;
		double min = 1e16;
		for (int i = 0; i < m_ReadData.size(); i++) {
			if (m_ReadData[i][index] > max) {
				max = m_ReadData[i][index];
			}
			else if (m_ReadData[i][index] < min) {
				min = m_ReadData[i][index];
			}
		}

		for (int i = 0; i < m_ReadData.size(); i++) {
			m_ReadData[i][index] -= min;
			m_ReadData[i][index] /= (max - min);
		}
	}
}

// �ҷ� �� csv ���ϵ��� ������� �ٲ�
void CClusterLayer::ChangeDataset() {
	if (path == "dartboard1.csv") {
		path = "dartboard2.csv";
	}
	else if (path == "dartboard2.csv") {
		path = "smile2.csv";
	}
	else if (path == "smile2.csv") {
		path = "zelnik1.csv";
	}
	else if (path == "zelnik1.csv") {
		path = "zelnik6.csv";
	}
	else if (path == "zelnik6.csv") {
		path = "square3.csv";
	}
	else {
		path = "dartboard1.csv";
	}
}

// Normalize �Լ��� ���� ����ȭ�� csv �����͵���
// GenerateData �Լ��� ���� ���̾ �׷���
void CClusterLayer::GenerateData()
{
	for (auto& Child : m_Children)
		delete Child;
	m_Children.clear();
	m_Point.clear();

	int index1, index2;
	index1 = rand() % (m_Head.size() - 1);
	index2 = rand() % (m_Head.size() - 1);
	while (index2 == index1) {
		index2 = rand() % (m_Head.size() - 1);
	}

	double x, y;
	for (int i = 0; i < m_ReadData.size(); i++) {
		double xx = m_ReadData[i][index1];
		double yy = m_ReadData[i][index2];

		x = (400 * xx) * 0.8 + 40;
		y = (400 - 400 * yy) * 0.8 + 40;

		CKhuGleSprite* Point = new CKhuGleSprite(GP_STYPE_ELLIPSE, GP_CTYPE_DYNAMIC, CKgLine(CKgPoint((int)x - 2, (int)y - 2), CKgPoint((int)x + 2, (int)y + 2)),
			KG_COLOR_24_RGB(255, 255, 255), true, 30);

		m_Point.push_back(Point);
		AddChild(Point);
	}
}

// R Ű�� ������ �����
// �������� points���� ������ (���� �ڵ� �̿�)
void CClusterLayer::GenerateRandomData(int nCnt)
{
	unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_real_distribution<double> uniform_dist(0, 1);

	for (auto& Child : m_Children)
		delete Child;
	m_Children.clear();
	m_Point.clear();

	int m_nClusterNum = rand() % 7 + 3;

	for (int k = 0; k < m_nClusterNum; ++k)
	{
		double mean1 = uniform_dist(generator);
		double mean2 = uniform_dist(generator);
		double sigma1 = uniform_dist(generator) / 10.;
		double sigma2 = uniform_dist(generator) / 10.;
		double rotate = uniform_dist(generator) * Pi;
		std::normal_distribution<double> normal_dist1(mean1, sigma1);
		std::normal_distribution<double> normal_dist2(mean2, sigma2);

		double x, y;

		for (int i = 0; i < nCnt; i++)
		{
			double xx = normal_dist1(generator);
			double yy = normal_dist2(generator);

			x = (xx - mean1) * cos(rotate) - (yy - mean2) * sin(rotate) + mean1;
			y = (xx - mean1) * sin(rotate) + (yy - mean2) * cos(rotate) + mean2;

			x = (x * m_nW - m_nW / 2) * 0.6 + m_nW / 2;
			y = (y * m_nH - m_nH / 2) * 0.6 + m_nH / 2;

			CKhuGleSprite* Point = new CKhuGleSprite(GP_STYPE_ELLIPSE, GP_CTYPE_DYNAMIC, CKgLine(CKgPoint((int)x - 2, (int)y - 2), CKgPoint((int)x + 2, (int)y + 2)),
				KG_COLOR_24_RGB(255, 255, 255), true, 30);

			m_Point.push_back(Point);
			AddChild(Point);
		}
	}
}

class CCorrelationClustering : public CKhuGleWin {
public:
	CKhuGleScene *m_pClusteringScene;
	CClusterLayer *m_pClusteringLayer;

	bool m_bCorrelationScene;

	CCorrelationClustering(int nW, int nH);
	virtual ~CCorrelationClustering() {
		m_pScene = nullptr;
		delete m_pClusteringScene;
	}
	void Update();
};

CCorrelationClustering::CCorrelationClustering(int nW, int nH)
	: CKhuGleWin(nW, nH) {
	m_pClusteringScene = new CKhuGleScene(640, 480, KG_COLOR_24_RGB(100, 100, 150));

	m_pClusteringLayer = new CClusterLayer(400, 400, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(120, 40));
	m_pClusteringScene->AddChild(m_pClusteringLayer);

	m_pScene = m_pClusteringScene;
}

void CCorrelationClustering::Update()
{

	// S�� ������ Ŭ�����͸� ����
	if(m_bKeyPressed['S'])
	{
		std::vector<CKhuGleSprite*> corePoints;
		std::vector<CKhuGleSprite*> noisePoints;
		int clusterCnt = 0;
		std::vector<std::vector<CKhuGleSprite*>> clusterList;

		// �߽����� ã�� corePoints ���Ϳ� ����
		for (auto& Point : m_pClusteringLayer->m_Point) {
			int inRadiusCnt = 0;
			for (auto& otherPoint : m_pClusteringLayer->m_Point) {
				double distance = (Point->m_Center.x - otherPoint->m_Center.x)
					* (Point->m_Center.x - otherPoint->m_Center.x)
					+ (Point->m_Center.y - otherPoint->m_Center.y)
					* (Point->m_Center.y - otherPoint->m_Center.y);
				distance = sqrt(distance);
				if (distance <= m_pClusteringLayer->radiusOfNeighborhood) {
					inRadiusCnt++;
				}
				if (inRadiusCnt >= m_pClusteringLayer->minimumOfNeighborhood) {
					corePoints.push_back(Point);
					break;
				}
			}
			if (inRadiusCnt < m_pClusteringLayer->minimumOfNeighborhood) {
				noisePoints.push_back(Point);
			}
		}

		clusterCnt = 0;
		// ã�Ƴ� �߽������� cluster�� ������
		// clusterList�� �� component�� �Ǵ� ���ʹ�, �� �ϳ��� Ŭ�����͸� �ǹ�
		while (!corePoints.empty()) {
			// corePoints �� �� ���� �����ͼ�
			// clusterList�� ���� ���� ����Ʈ�� ����
			int index = 0;
			clusterList.push_back({});
			clusterList[clusterCnt].push_back(corePoints[0]);

			// ������ corePoints �� �� ����
			// corePoints���� ������
			CKhuGleComponent* it = *std::find(corePoints.begin(), corePoints.end(), corePoints[0]);
			corePoints.erase(remove(corePoints.begin(), corePoints.end(), corePoints[0]));

			// �� Ŭ�������� ��� ���� ���Ͽ�
			// ������ �߽������� �ش� Ŭ�����Ϳ� �߰�
			while (index < clusterList[clusterCnt].size()) {
				int corePointsIndex = 0;

				// ������ �߽����� ���� ó��
				if (corePoints.empty()) break;
				for (auto& otherPoint : corePoints) {
					if (clusterList[clusterCnt][index] == otherPoint) continue;
					double distance = (clusterList[clusterCnt][index]->m_Center.x - otherPoint->m_Center.x)
						* (clusterList[clusterCnt][index]->m_Center.x - otherPoint->m_Center.x)
						+ (clusterList[clusterCnt][index]->m_Center.y - otherPoint->m_Center.y)
						* (clusterList[clusterCnt][index]->m_Center.y - otherPoint->m_Center.y);
					distance = sqrt(distance);

					// Ŭ������ �� �߽����� ������ �ٸ� �߽�������
					// Ŭ������ ����Ʈ�� ����
					if (distance <= m_pClusteringLayer->radiusOfNeighborhood) {
						clusterList[clusterCnt].push_back(otherPoint);
						std::swap(corePoints[corePointsIndex], otherPoint);
						corePointsIndex++;
					}
				}

				// ������ ó���ߴ� �߽������� corePoints ���Ϳ��� �� ���� ������
				corePoints.erase(corePoints.begin(), corePoints.begin() + corePointsIndex);
				index++;
			}
			clusterCnt++;
		}

		// �߽����� �ƴ� ����� ���� ó��
		// ������ �߿�����, Ŭ�����Ϳ� �����ٸ�
		// border point�� �����Ͽ� clusterList�� ���Խ�Ŵ
		for (int i = 0; i < clusterCnt; i++) {
			for (int j = 0; j < clusterList[i].size(); j++) {
				if (noisePoints.empty()) break;
				for (auto& noisePoint : noisePoints) {
					if (noisePoints.empty()) break;
					double distance = (noisePoint->m_Center.x - clusterList[i][j]->m_Center.x)
						* (noisePoint->m_Center.x - clusterList[i][j]->m_Center.x)
						+ (noisePoint->m_Center.y - clusterList[i][j]->m_Center.y)
						* (noisePoint->m_Center.y - clusterList[i][j]->m_Center.y);
					if (distance <= m_pClusteringLayer->radiusOfNeighborhood) {
						clusterList[i].push_back(noisePoint);
						noisePoints.erase(remove(noisePoints.begin(), noisePoints.end(), noisePoint));
						break;
					}
				}
			}
		}

		// �ð�ȭ�� ���� �� ����
		KgColor24 color;
		for (int i = 0; i < clusterCnt; i++) {
			for (auto& Point : clusterList[i]) {
				switch (i) {
				case 0:
					color = KG_COLOR_24_RGB(0, 0, 255);
					break;
				case 1:
					color = KG_COLOR_24_RGB(0, 255, 0);
					break;
				case 2:
					color = KG_COLOR_24_RGB(255, 0, 0);
					break;
				case 3:
					color = KG_COLOR_24_RGB(0, 255, 255);
					break;
				case 4:
					color = KG_COLOR_24_RGB(255, 0, 255);
					break;
				case 5:
					color = KG_COLOR_24_RGB(255, 255, 0);
					break;
				default:
					color = KG_COLOR_24_RGB(255, 255, 255);
				}
				Point->m_fgColor = color;
			}
		}

		// �������̶�� -> ������
		for (auto& noisePoint : noisePoints) {
			noisePoint->m_fgColor = KG_COLOR_24_RGB(0, 0, 0);
		}
	}
	m_bKeyPressed['S'] = false;

	// N�� �����ٸ�
	// ���� �� csv �����͵��� ���̾� �� �׸�
	if(m_bKeyPressed['N'])
	{
		m_pClusteringLayer->ChangeDataset();
		m_pClusteringLayer->ReadData();
		m_pClusteringLayer->Normalization();
		m_pClusteringLayer->GenerateData();

		m_bKeyPressed['N'] = false;
	}
	// R�� �����ٸ�
	// �������� ������ point�� ������
	else if (m_bKeyPressed['R'])
	{
		m_pClusteringLayer->GenerateRandomData(50);
		m_bKeyPressed['R'] = false;
	}

	m_pScene->Render();
	DrawSceneTextPos("DBSCAN Clustering", CKgPoint(0, 0));
	
	CKhuGleWin::Update();
}

int main()
{
	CCorrelationClustering *pCorrelationClustering = new CCorrelationClustering(640, 480);
	KhuGleWinInit(pCorrelationClustering);

	return 0;
}