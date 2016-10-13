/***************************************************************************
 SocNetV: Social Network Visualizer
 version: 2.2
 Written in Qt
 
                         vertex.h  -  description
                             -------------------
    copyright            : (C) 2005-2016 by Dimitris B. Kalamaras
    email                : dimitris.kalamaras@gmail.com
 ***************************************************************************/

/*******************************************************************************
*     This program is free software: you can redistribute it and/or modify     *
*     it under the terms of the GNU General Public License as published by     *
*     the Free Software Foundation, either version 3 of the License, or        *
*     (at your option) any later version.                                      *
*                                                                              *
*     This program is distributed in the hope that it will be useful,          *
*     but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*     GNU General Public License for more details.                             *
*                                                                              *
*     You should have received a copy of the GNU General Public License        *
*     along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
********************************************************************************/

#ifndef VERTEX_H
#define VERTEX_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QList>
#include <QPointF>
#include <map>

using namespace std;

class QPointF;
class Graph;


typedef QHash<int,QString> H_IntToStr;
typedef QList<int> ilist;
typedef QPair <float, bool> pair_f_b;
typedef QPair <int, pair_f_b > rel_w_bool;
typedef QHash < int, rel_w_bool > H_edges;
typedef QHash <QString, int> H_StrToInt;

class Vertex : public QObject{
    Q_OBJECT

public:

    Vertex(Graph* parent,
           const long int &name,
           const int &val,
           const int &relation,
           const int &size,
           const QString &color,
           const QString &numColor,
           const int &numSize,
           const QString &label,
           const QString &labelColor,
           const int &labelSize,
           const QPointF &p,
           const QString &shape);

    Vertex(const long int &name);

    ~Vertex();

    long int name() const { return m_name; }

    void setName (const long int &name) { m_name=name; }

    void setEnabled (const bool &flag ) { m_enabled=flag; }
    bool isEnabled () const { return m_enabled; }

    void relationSet(int) ;

    void edgeAddTo (const long int &v2, const float &weight);
    void edgeAddFrom(const long int &v1, const float &weight);

    void changeOutEdgeWeight (long int target, float weight);

    void setOutEdgeEnabled (long int, bool);

    void edgeRemoveTo (long int target);
    void edgeRemoveFrom(long int source);

    QHash<int,float>* outEdgesEnabledHash();
    QHash<int,float>* inEdgesEnabledHash();
    QHash<int,float>* reciprocalEdgesHash();
    QList<int> neighborhoodList();

    long int outEdges();
    long int outEdgesConst() const ;

    long int inEdges();
    long int inEdgesConst() const ;

    long int degreeOut();
    long int outDegreeConst();
    long int degreeIn();
    long int inDegreeConst();
    long int localDegree();

    /* sets eccentricity */
    void setEccentricity (float c){ m_Eccentricity=c;}
    float eccentricity() { return m_Eccentricity;}

    /* Returns true if there is an outLink from this vertex */
    bool isOutLinked() { return (outEdges() > 0) ? true:false;}
    float hasEdgeTo(const long int &v, const bool &allRelations=false);

    /* Returns true if there is an outLink from this vertex */
    bool isInLinked() { return  (inEdges() > 0) ? true:false;}
    float hasEdgeFrom (const long int &v, const bool &allRelations=false);

    bool isIsolated() { return !(isOutLinked() | isInLinked()) ; }
    void setIsolated(bool isolated) {m_isolated = isolated; }

    void edgeFilterByWeight(float m_threshold, bool overThreshold);
    //	void filterEdgesByColor(float m_threshold, bool overThreshold);
    void edgeFilterByRelation(int relation, bool status);
    void edgeFilterUnilateral(const bool &toggle=false,
                              const bool &allRelations=false);

    void setSize(const int &size ) { m_size=size; }
    int size()  const { return m_size; }

    void setShape(const QString &shape) { m_shape=shape; }
    QString shape() const { return m_shape; }

    void setColor(const QString &color) { m_color=color; }
    QString color() const { return m_color; }
    QString colorToPajek();

    void setNumberColor (const QString &color) { m_numberColor = color; }
    QString numberColor() const { return m_numberColor; }

    void setNumberSize (const int &size) { m_numberSize=size; }
    int numberSize() const { return m_numberSize; }

    void setNumberDistance (const int &distance) { m_numberDistance=distance; }
    int numberDistance() const { return m_numberDistance; }

    void setLabel (const QString &label) { m_label=label; }
    QString label() const { return m_label; }

    void setLabelColor (const QString &labelColor) { m_labelColor=labelColor; }
    QString labelColor() const { return m_labelColor; }

    void setLabelSize(const int &size) { m_labelSize=size; }
    int labelSize() const { return m_labelSize; }

    void setLabelDistance (const int &distance) { m_labelDistance=distance; }
    int labelDistance() const { return m_labelDistance; }

    void setX(const float &x) { m_x=x; }
    float x() const { return m_x; }

    void setY(const float &y) { m_y=y; }
    float y() const { return m_y; }

    QPointF pos () const { return QPointF ( x(), y() ); }

    //returns displacement vector
    QPointF & disp() { return m_disp; }

    void set_dispX (float x) { m_disp.rx() = x ; }
    void set_dispY (float y) { m_disp.ry() = y ; }


    void setOutLinkColor(const long int &v2,
                         const QString &color) { outLinkColors[v2]=color; }
    QString outLinkColor(const long int &v2) {
        return ( outLinkColors.contains(v2) ) ? outLinkColors.value(v2) : "black";
    }


    void setOutEdgeLabel(const long int &v2,
                         const QString &label) { m_outEdgeLabels[v2]=label; }
    QString outEdgeLabel(const long int &v2) const {
        return ( m_outEdgeLabels.contains(v2) ) ? m_outEdgeLabels.value(v2) : QString::null;
    }


    void setDelta (float c){ m_delta=c;} 		/* Sets vertex pair dependancy */
    float delta() { return m_delta;}		/* Returns vertex pair dependancy */

    void clearPs()	;

    void appendToPs(long  int vertex ) ;
    ilist Ps(void);

    void setDC (float c){ m_DC=c;} 	/* Sets vertex Degree Centrality*/
    void setSDC (float c ) { m_SDC=c;}	/* Sets standard vertex Degree Centrality*/
    float DC() { return m_DC;}		/* Returns vertex Degree Centrality*/
    float SDC() { return m_SDC;}		/* Returns standard vertex Degree Centrality*/

    void setCC (float c){ m_CC=c;}		/* sets vertex Closeness Centrality*/
    void setSCC (float c ) { m_SCC=c;}	/* sets standard vertex Closeness Centrality*/
    float CC() { return m_CC;}		/* Returns vertex Closeness Centrality*/
    float SCC() { return m_SCC; }		/* Returns standard vertex Closeness Centrality*/

    void setIRCC (float c){ m_IRCC=c;}		/* sets vertex IRCC */
    void setSIRCC (float c ) { m_SIRCC=c;}	/* sets standard vertex IRCC */
    float IRCC() { return m_IRCC;}		/* Returns vertex IRCC */
    float SIRCC() { return m_SIRCC; }		/* Returns standard vertex IRCC*/

    void setBC(float c){ m_BC=c;}		/* sets s vertex Betweenness Centrality*/
    void setSBC (float c ) { m_SBC=c;}	/* sets standard vertex Betweenness Centrality*/
    float BC() { return m_BC;}		/* Returns vertex Betweenness Centrality*/
    float SBC() { return m_SBC; }		/* Returns standard vertex Betweenness Centrality*/

    void setSC (float c){ m_SC=c;}  	/* sets vertex Stress Centrality*/
    void setSSC (float c ) { m_SSC=c;}	/* sets standard vertex Stress Centrality*/
    float SC() { return m_SC;}		/* Returns vertex Stress Centrality*/
    float SSC() { return m_SSC; }		/* Returns standard vertex Stress Centrality*/

    void setEC(float dist) { m_EC=dist;}	/* Sets max Geodesic Distance to all other vertices*/
    void setSEC(float c) {m_SEC=c;}
    float EC() { return m_EC;}		/* Returns max Geodesic Distance to all other vertices*/
    float SEC() { return m_SEC;}

    void setPC (float c){ m_PC=c;}		/* sets vertex Power Centrality*/
    void setSPC (float c ) { m_SPC=c;}	/* sets standard vertex Power Centrality*/
    float PC() { return m_PC;}		/* Returns vertex Power Centrality*/
    float SPC() { return m_SPC; }		/* Returns standard vertex Power Centrality*/

    void setIC (float c){ m_IC=c;}		/* sets vertex Information Centrality*/
    void setSIC (float c ) { m_SIC=c;}	/* sets standard vertex Information Centrality*/
    float IC() { return m_IC;}		/* Returns vertex Information  Centrality*/
    float SIC() { return m_SIC; }		/* Returns standard vertex Information Centrality*/

    void setDP (float c){ m_DP=c;} 	/* Sets vertex Degree Prestige */
    void setSDP (float c ) { m_SDP=c;}	/* Sets standard vertex Degree Prestige */
    float DP() { return m_DP;}		/* Returns vertex Degree Prestige */
    float SDP() { return m_SDP;}		/* Returns standard vertex Degree Prestige */

    void setPRP (float c){ m_PRC=c;}		/* sets vertex PageRank*/
    void setSPRP (float c ) { m_SPRC=c;}	/* sets standard vertex PageRank*/
    float PRP() { return m_PRC;}		/* Returns vertex PageRank */
    float SPRP() { return m_SPRC; }		/* Returns standard vertex PageRank*/

    void setPP (float c){ m_PP=c;}		/* sets vertex Proximity Prestige */
    void setSPP (float c ) { m_SPP=c;}	/* sets standard vertex Proximity Prestige */
    float PP() { return m_PP;}		/* Returns vertex Proximity Prestige */
    float SPP() { return m_SPP; }		/* Returns standard vertex Proximity Prestige */

    float CLC() { return m_CLC;	}
    void setCLC(float clucof)  { m_CLC=clucof; m_hasCLC=true; }
    bool hasCLC() { 	return m_hasCLC; }


    int cliques (const int &size);

    bool cliqueAdd (const QString &clique, const int &size);

    void clearCliques() {
        m_cliques.clear();
    }

    //hold all outbound and inboud edges of this vertex.
    H_edges m_outEdges, m_inEdges;
signals:
    void setEdgeVisibility (int, int, int, bool);

protected:

private:
    Graph *parentGraph;
    ilist myPs;
    long int m_name,  m_outEdgesCounter, m_inEdgesCounter, m_outDegree, m_inDegree, m_localDegree;
    float m_Eccentricity;
    int m_value, m_size, m_labelSize, m_numberSize, m_numberDistance, m_labelDistance;
    int m_curRelation;
    H_StrToInt m_cliques;
    QHash<int,float>* m_reciprocalEdges;
    QList<int> m_neighborhoodList;
    bool m_reciprocalLinked, m_enabled, m_hasCLC, m_isolated;
    QString m_color, m_numberColor, m_label, m_labelColor, m_shape;
    QPointF m_disp;
    H_IntToStr outLinkColors, m_outEdgeLabels;
    //FIXME vertex coords

    double m_x, m_y;
    float m_CLC;
    float m_delta, m_EC, m_SEC;
    float m_DC, m_SDC, m_DP, m_SDP, m_CC, m_SCC, m_BC, m_SBC, m_IRCC, m_SIRCC, m_SC, m_SSC;
    float m_PC, m_SPC, m_SIC, m_IC, m_SPRC, m_PRC;
    float m_PP, m_SPP;

};

#endif
