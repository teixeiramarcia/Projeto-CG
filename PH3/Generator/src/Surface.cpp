#include"../headers/Surface.h"

using namespace std;

Surface::Surface(const string &filename,int tesselation)
{
    this->readPatchFile(filename);
    this->tesselation = tesselation;
}

Surface::~Surface(){}

template<typename T>
vector<vector<T>> Surface::multiplyMatrices(const vector<vector<T>> &m1, const vector<vector<T>> &m2)
{
    vector<vector<T>> result;
    const int m1NCol = m1[0].size();
    const int m2NCol = m2[0].size();

    if(m1NCol!=m2.size())
        return result;

    // a cada linha m1
    for(int i=0; i<m1.size() ; i++)
    {
        vector<T> linha;
        // a cada coluna m2
        for(int j=0;j<m2NCol;j++)
        {
            T sumProduct;
            // sum product line matrix1 with col matrix2
            for(int times=0;times<m1NCol;times++)
            {
                T elM1((m1[i][times]));
                T elM2((m2[times][j]));
                sumProduct = sumProduct + (elM1*elM2);
            }
            linha.push_back(sumProduct);
        }
        result.push_back(linha);
    }
    return result;
}

// calcula um ponto da patch bezier
Point3D Surface::bezierPatch(const double u,const double v,const vector<vector<Point3D>> &controlPoints)
{
    const vector<vector<Point3D>> bezierM = {
        { Point3D(-1.0) , Point3D(3.0)  , Point3D(-3.0) , Point3D(1.0) },
        { Point3D(3.0)  , Point3D(-6.0) , Point3D(3.0)  , Point3D(0.0) },
        { Point3D(-3.0) , Point3D(3.0)  , Point3D(0.0)  , Point3D(0.0) },
        { Point3D(1.0)  , Point3D(0.0)  , Point3D(0.0)  , Point3D(0.0) }
    };
    const vector<vector<Point3D>> mfirst = { { Point3D(u*u*u), Point3D(u*u), Point3D(u), Point3D(1) } };
    const vector<vector<Point3D>> mlast = {
        { Point3D(v*v*v) },
        { Point3D(v*v)   },
        { Point3D(v)     },
        { Point3D(1.0)   }
    };

    vector<vector<Point3D>> multi;
    multi = multiplyMatrices<Point3D>(mfirst,bezierM);
    multi = multiplyMatrices<Point3D>(multi,controlPoints);
    multi = multiplyMatrices<Point3D>(multi,bezierM);
    multi = multiplyMatrices<Point3D>(multi,mlast);

    return multi[0][0];
}

// cria triangulos a partir de 2 vectores de pontos
vector<Triangle> Surface::getTrianglesIn2Vectors(const vector<Point3D> &v1,const vector<Point3D> &v2)
{
    vector<Triangle> triangles;
    int size = v1.size();
    int sections = size-1;

    for(int i=0;i<sections;i++)
    {
        Triangle t1(v1[i],v2[i],v2[i+1]);
        triangles.push_back(t1);
        Triangle t2(v1[i],v2[i+1],v1[i+1]);
        triangles.push_back(t2);
    }

    return triangles;
}

// calcula todos os pontos de uma patch bezier
vector<vector<Point3D>> Surface::calculatePatchPoints(const vector<vector<Point3D>> &controlPoints)
{
    const double gap = 1.0/this->tesselation;
    double u,v;
    int nPoints = this->tesselation+1;
    vector<vector<Point3D>> patchPoints;

    for(int i=0;i<nPoints;i++)
    {
        vector<Point3D> linha;
        u = i+1>=nPoints? 1.0 : gap*i;

        for(int j=0;j<nPoints;j++)
        {
            v = j+1>=nPoints? 1.0 : gap*j;
            linha.push_back(bezierPatch(u,v,controlPoints));
        }

        patchPoints.push_back(linha);
        linha.clear();
    }

    return patchPoints;
}

// cria triangulos para uma patch bezier
vector<Triangle> Surface::getPatchTriangles(const vector<vector<Point3D>> &controlPoints)
{
    vector<Triangle> triangles;
    vector<vector<Point3D>> patchPoints = this->calculatePatchPoints(controlPoints);

    for(int times=0;times<patchPoints.size()-1;times++)
    {
        auto stripe = this->getTrianglesIn2Vectors(patchPoints[times],patchPoints[times+1]);
        triangles.insert(triangles.end(),stripe.begin(),stripe.end());
    }

    return triangles;
}

// cria triangulos da superficie a partir das patches bezier
vector<Triangle> Surface::getSurfaceTriangles()
{
    vector<Triangle> surface;

    for(auto &controlPoints: this->patchesCtrlP)
    {
        auto triangles = getPatchTriangles(controlPoints);
        surface.insert(surface.end(),triangles.begin(),triangles.end());
    }

    return surface;
}

// string com formato impressao para ficheiro output
string Surface::toString()
{
    string s;

    vector<Triangle> triangles = this->getSurfaceTriangles();

    int nPontos = triangles.size()*3;
    s.append(to_string(nPontos)+"\n");

    for(auto &t : triangles)
        s.append(t.toString());

    return s;
}

// obtencao matriz pontos controlo de uma patch
vector<vector<Point3D>> Surface::getPatchMatrix(const vector<int> &indices,const vector<Point3D> &pontosControlo)
{
    vector<vector<Point3D>> matrix;

    for(int curve=0 ; curve<4; curve++)
    {
        vector<Point3D> linha;
        for(int nPoints=0; nPoints<4; nPoints++)
            linha.push_back(pontosControlo[indices[curve*4+nPoints]]);
        matrix.push_back(linha);
    }
    return matrix;
}

// Funcao leitura ficheiro das patches
void Surface::readPatchFile(string filename)
{
	ifstream fin(filename);
    int nPatches;
    int nPontosControlo;
    vector<vector<int>> indicesPatches;
    vector<Point3D> pontosControlo;
    cout << filename << endl;
    // falha abertura ficheiro
    if(!fin.is_open())
    {
        cout << endl << "FALHA ABERTURA FICHEIRO" << endl;
        exit(1);
    }

    // ler numero patches
    fin >> nPatches; fin.get();

    // ler indices patches
    for(int patch=0; patch<nPatches; patch++)
    {
        vector<int> patchIndices;
        for(int index=0; index<16; index++)
        {
            int indice;
            fin >> indice; fin.get();
            patchIndices.push_back(indice);
        }
        indicesPatches.push_back(patchIndices);
    }

	// ler numero pontos de controlo
    fin >> nPontosControlo; fin.get();

    //ler pontos controlo
    for(int indice=0; indice<nPontosControlo; indice++)
    {
        double x,y,z;
        fin >> x; fin.get();
        fin >> z; fin.get();
        fin >> y; fin.get();
        pontosControlo.push_back(Point3D(x,y,z));
    }

    fin.close();

    for(int i=0 ; i<nPatches ; i++)
        this->patchesCtrlP.push_back( getPatchMatrix(indicesPatches[i],pontosControlo) );
}
