/***************************************************************************
 SocNetV: Social Network Visualizer 
 version: 2.0
 Written in Qt

                        node.cpp  -  description
                        -------------------
    copyright            : (C) 2005-2015 by Dimitris B. Kalamaras
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


#include "node.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QtGlobal>
#include <QDebug>
#include "graphicswidget.h"
#include "edge.h"
#include "nodelabel.h"
#include "nodenumber.h"
#include <math.h>



Node::Node(GraphicsWidget* gw, const int &num, const int &size,
            const QString &color, const QString &shape,
           const bool &showNumbers, const bool &numbersInside,
           const QString &numberColor, const int &numberSize,
           const int &numDistance,
           const bool &showLabels,  const QString &label, const QString &labelColor,
           const int &labelSize, const int &labelDistance,
           QPointF p
            ) : graphicsWidget (gw)
{
	Q_UNUSED(p);
	graphicsWidget->scene()->addItem(this); //Without this nodes don't appear on the screen...

    setFlags(ItemSendsGeometryChanges | ItemIsSelectable | ItemIsMovable );
    //setCacheMode(QGraphicsItem::ItemCoordinateCache); //QT < 4.6 if a cache mode is set, nodes do not respond to hover events

	setCacheMode(QGraphicsItem::NoCache); //QT < 4.6 if a cache mode is set, nodes do not respond to hover events
    setAcceptHoverEvents(true);

	m_num=num;
	m_size=size;

	m_shape=shape;
    m_col_str=color;
    m_col=QColor(color);

    m_hasNumber=showNumbers;
    m_hasNumberInside = numbersInside;
    m_numSize = numberSize;
    m_numColor = numberColor;
    m_numberDistance=numDistance;

    m_hasLabel=showLabels;
    m_labelText = label;
    m_labelSize = labelSize;
    m_labelColor = labelColor;

    m_ld=labelDistance;

    if (m_hasLabel) {
        addLabel();
    }

    if (!m_hasNumberInside && m_hasNumber) {
        addNumber();
    }

    setShape(m_shape);

    qDebug()<< "Node: constructor: initial position at: "
			<< this->x()<<", "<<this->y()
			<< " Will move at: "<< p.x()<<", "<<p.y();;

} 



/** 
	Used by MW::slotChangeNodeColor
*/
void Node::setColor(QString str) {
	prepareGeometryChange();
	m_col=QColor(str);
    update();
}

/** 
    Used by MW::slotEditNodeFind()
*/
void Node::setColor(QColor color){
    prepareGeometryChange();
	m_col=color;
    m_col_str = m_col.name();
    update();
}


QString Node::color() {
	return m_col_str;
}


/** Sets the size of the node */
void Node::setSize(int size){
	qDebug("Node: setSize()");
 	prepareGeometryChange();
	m_size=size;
	foreach (Edge *edge, inEdgeList) {
		qDebug("Node: updating edges in inEdgeList");
		edge->setEndOffset(size);
	}
	foreach (Edge *edge, outEdgeList) {
		qDebug("Node: updating edges in outEdgeList");
		edge->setStartOffset(size);
	}
	setShape(m_shape);
}



/**  Used by MainWindow::findNode() and Edge::Edge()  */
int Node::size(){
	qDebug("size()");
	return m_size;  
}


/**  Called every time the user needs to change the shape of an node. */
void Node::setShape(const QString shape) {
	qDebug("Node: setShape()");
	prepareGeometryChange();
	m_shape=shape;
	qDebug ("Node: setShape(): node is at x=%f and y=%f", x(), y());

    m_path = new QPainterPath;
    if ( m_shape == "circle") {
        m_path->addEllipse (-m_size, -m_size, 2*m_size, 2*m_size);
        if (m_hasNumberInside && m_hasNumber)
            m_path->addText(-m_size/2,m_size/2,QFont("Times", m_numSize, QFont::Normal), QString::number(m_num) );
    }
    else if ( m_shape == "ellipse") {
        m_path->addEllipse(-m_size, -m_size, 2*m_size, 1.5* m_size);
        if (m_hasNumberInside && m_hasNumber)
            m_path->addText(-m_size/2,m_size/2,QFont("Times", m_numSize, QFont::Normal), QString::number(m_num) );
    }
    else if ( m_shape == "box" || m_shape == "rectangle"  ) {  //rectangle: for GraphML compliance
        m_path->addRect (-m_size , -m_size , 1.8*m_size , 1.8*m_size );
        if (m_hasNumberInside && m_hasNumber)
            m_path->addText(-m_size/2,m_size/2,QFont("Times", m_numSize, QFont::Normal), QString::number(m_num) );
    }
    else if (m_shape == "roundrectangle"  ) {  //roundrectangle: GraphML only
        m_path->addRoundedRect (-m_size , -m_size , 1.8*m_size , 1.8*m_size, 60.0, 60.0, Qt::RelativeSize );
        if (m_hasNumberInside && m_hasNumber)
            m_path->addText(-m_size/2,m_size/2,QFont("Times", m_numSize, QFont::Normal), QString::number(m_num) );
    }
    else if ( m_shape == "triangle") {
        m_poly_t=new QPolygon(3);
        m_poly_t -> setPoints (3,  0,-m_size,  -m_size,m_size, m_size,+m_size);
        m_path->addPolygon(*m_poly_t);
        if (m_hasNumberInside && m_hasNumber)
        m_path->addText(-m_size/2,m_size/2,QFont("Times", m_numSize, QFont::Normal), QString::number(m_num) );
        m_path->closeSubpath();
    }
    else if ( m_shape == "diamond"){
        m_poly_d=new QPolygon(4);
        m_poly_d -> setPoints (4, 0,-m_size,  -m_size,0,       0,+m_size,     +m_size,0);
        m_path->addPolygon(*m_poly_d);
        if (m_hasNumberInside && m_hasNumber)
            m_path->addText(-m_size/2,m_size/2,QFont("Times", m_numSize, QFont::Normal), QString::number(m_num) );
        m_path->closeSubpath();
    }
    update();
}



/* 
*	Returns the shape of the node as a path (an accurate outline of the item's shape)
*	Used by the collision algorithm in collidesWithItem() 
*/
QPainterPath Node::shape() const {
 	//qDebug ("Node: shape()");
	return (*m_path);
}


/*
 *  Returns the bounding rectangle of the node
 *  That is the rectangle where all painting will take place.
 */
QRectF Node::boundingRect() const {
	qreal adjust = 6;
	return QRectF(-m_size -adjust , -m_size-adjust , 2*m_size+adjust , 2*m_size +adjust);
	

}


/** 
	Does the actual painting. 
	Called by GraphicsView in every update() 
*/
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
//	painter->setClipRect( option->exposedRect );

    //if the node is being dragged around, darken it!
	if (option->state & QStyle::State_Selected) {
		//qDebug()<< " node : selected ";
		painter->setBrush(m_col.dark(150));
	}
	else if (option->state & QStyle::State_MouseOver) {
		//qDebug()<< " node : mouse over";	
		painter->setBrush(m_col.dark(150));
		setZValue(255);		
	}
	//else if (option->state & QStyle::State_Sunken) {
		//qDebug()<< " node : sunken ";
		//setZValue(255);
		//painter->setBrush(m_col_dark.dark(160));
	//}
 	else { //no, just paint it with the usual color.
 		//qDebug()<< " node : nothing";
		setZValue(254);		
		painter->setBrush(m_col);
	}
	painter->setPen(QPen(Qt::black, 0));

	painter->drawPath (*m_path);
}





/** 
 *	Propagates the changes to connected elements, i.e. edges, numbers, etc. 
 *  Checks if the node is inside the scene.
 */
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
	QPointF newPos = value.toPointF();

    switch (change) {
    case ItemPositionHasChanged :
    {
        //setCacheMode( QGraphicsItem::ItemCoordinateCache );
        foreach (Edge *edge, inEdgeList)  //Move each inEdge of this node
            edge->adjust();
        foreach (Edge *edge, outEdgeList) //Move each outEdge of this node
            edge->adjust();
        //Move its graphic number
        if ( m_hasNumber )
        {
            if (!m_hasNumberInside) 	{ //move it outside
                m_number -> setZValue(254);
                m_number -> setPos( m_size+m_numberDistance, 0);
            }
        }
        if (m_hasLabel) {
            m_label->setPos( -2, m_ld+m_size);
        }
        if ( newPos.x() !=0 && newPos.y() != 0 ){
            graphicsWidget->nodeMoved(nodeNumber(), (int) newPos.x(), (int) newPos.y());
        }
        else qDebug()<<  "Node: ItemChange():  Not emitting nodeMoved. Node "
                      << nodeNumber()<<" is at 0,0";
        break;
    }
    case ItemEnabledHasChanged:{
        if (ItemEnabledHasChanged) {
            return 1;
        }
        else{
            return 0;
        }
    }
    case ItemVisibleHasChanged:
    {
        if (ItemVisibleHasChanged){
            return 1;
        }
        else{
            return 0;
        }
    }
    default:
    {
        break;
    }
    };
    return QGraphicsItem::itemChange(change, value);
}



/** handles the events of a click on a node */
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
}


void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	update();
	QGraphicsItem::mouseReleaseEvent(event);
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    foreach (Edge *edge, inEdgeList)
        edge->highlight(true);
    foreach (Edge *edge, outEdgeList)
        edge->highlight(true);
    QGraphicsItem::hoverEnterEvent(event);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    foreach (Edge *edge, inEdgeList)
        edge->highlight(false);
    foreach (Edge *edge, outEdgeList)
        edge->highlight(false);
    QGraphicsItem::hoverLeaveEvent(event);
}


void Node::addInLink( Edge *edge ) {
	qDebug() << "Node:  addInLink() for "<<  m_num;
	inEdgeList.push_back( edge); 
	//qDebug ("Node:  %i inEdgeList has now %i edges", m_num, inEdgeList.size());
}


void Node::deleteInLink( Edge *link ){
	qDebug () << "Node:  deleteInLink for "<< m_num;
	//qDebug ("Node: %i inEdgeList has %i edges", m_num, inEdgeList.size());
    inEdgeList.remove( link);
	//qDebug ("Node:  %i inEdgeList has now %i edges", m_num, inEdgeList.size());
}



void Node::addOutLink( Edge *edge ) {
	qDebug("Node: addOutLink()");
	outEdgeList.push_back( edge); 
//	qDebug ("Node: outEdgeList has now %i edges", outEdgeList.size());
}



void Node::deleteOutLink(Edge *link){
	qDebug () << "Node: deleteOutLink() from " <<  m_num;
//	qDebug ("Node: %i outEdgeList has %i edges", m_num, outEdgeList.size());
    outEdgeList.remove( link);
//	qDebug ("Node: %i outEdgeList has now %i edges", m_num, outEdgeList.size());
}



void Node::addLabel ()  {
    //Draw node labels
    m_label = new  NodeLabel (this, m_labelText, m_labelSize);
    m_label -> setDefaultTextColor (m_labelColor);
}



NodeLabel* Node::label(){
    if (!m_hasLabel) {
        addLabel();
    }
    return m_label;
}

void Node::deleteLabel(){
	qDebug ("Node: deleteLabel ");
	m_hasLabel=false;
	m_label->hide();
	graphicsWidget->removeItem(m_label);		
}


void Node::setLabelText ( QString label) {
    prepareGeometryChange();
    m_labelText = label;
    if (m_hasLabel)
        m_label->setPlainText(label);
    else
        addLabel();
    m_hasLabel=true;
}



void Node::setLabelVisibility(const bool &toggle) {
    if (toggle){
        if (m_hasLabel) {
            m_label->show();
        }
        else {
            addLabel();
        }
    }
    else {
        if (m_hasLabel) {
            m_label->hide();
        }
    }

    m_hasLabel=toggle;
}

void Node::setLabelSize(const int &size) {
    m_labelSize = size;
    if (!m_hasLabel) {
        addLabel();
    }
    m_label->setSize(m_labelSize);

}

/**
 * @brief Node::labelText
 * @return QString
 */
QString Node::labelText ( ) {
    return m_labelText;
}




void Node::addNumber () {
    qDebug()<<"Node::addNumber () " ;
    m_hasNumber=true;
    m_hasNumberInside = false;
    m_number= new  NodeNumber ( this, QString::number(m_num), m_numSize);
    m_number -> setDefaultTextColor (m_numColor);
    m_number -> setPos(m_size+m_numberDistance, 0);

}

NodeNumber* Node::number(){
    return m_number;
}


void Node::deleteNumber( ){
    qDebug () << "Node: deleteNumber ";
    if (m_hasNumber && !m_hasNumberInside) {
        m_number->hide();
        graphicsWidget->removeItem(m_number);
        m_hasNumber=false;
    }
}

void Node::setNumberVisibility(const bool &toggle) {
    qDebug() << "Node::setNumberVisibility() " << toggle;
    if (toggle) { //show
        if (!m_hasNumber) {
            m_hasNumber=toggle;
            if ( !m_hasNumberInside )
                addNumber();
            else {
                setShape(m_shape);
            }
        }
    }
    else { // hide
        deleteNumber();
        m_hasNumber=toggle;
        setShape(m_shape);
    }

}

void Node::setNumberInside (const bool &toggle){
    qDebug()<<"Node::setNumberInside() " << toggle;
    if (toggle) { // set number inside
        deleteNumber();
    }
    else {
        addNumber();
    }
   m_hasNumber = true;
   m_hasNumberInside = toggle;
   setShape(m_shape);
}


/**
 * @brief Node::setNumberSize
 * @param size
 */
void Node::setNumberSize(const int &size) {
    m_numSize = size;
    if (m_hasNumber && !m_hasNumberInside) {
        m_number->setSize(m_numSize);
    }
    else if (m_hasNumber && m_hasNumberInside) {
        setShape(m_shape);
    }
    else {
        // create a nodeNumber ?
    }

}


/**
 * @brief Node::setNumberColor
 * @param color
 */
void Node::setNumberColor(const QString &color) {
    m_numColor = color;
    if (m_hasNumber){
        if (m_hasNumberInside) {
            setShape(m_shape);
        }
        else {
           m_number -> setDefaultTextColor (m_numColor);
        }
    }

}

/**
 * @brief Node::setNumberDistance
 * @param distance
 */
void Node::setNumberDistance(const int &distance) {
    m_numberDistance = distance;
    if (m_hasNumber && !m_hasNumberInside) {
       m_number -> setPos( m_size+m_numberDistance, 0);
    }
    else if (m_hasNumber && m_hasNumberInside) {
       // do nothing
    }
    else {
        // create a nodeNumber ?
    }

}





 Node::~Node(){
    qDebug() << "\n\n\n *** ~Node() "<< nodeNumber();
    foreach (Edge *edge, inEdgeList) {
        qDebug("~Node: removing edges in inEdgeList");
        //edge->remove();
        graphicsWidget->removeItem(edge);
    }
    foreach (Edge *edge, outEdgeList) {
        qDebug("~Node: removing edges in outEdgeList");
        //edge->remove();
        graphicsWidget->removeItem(edge);
    }
    if ( m_hasNumber )
        this->deleteNumber();
    if ( m_hasLabel )
        this->deleteLabel();
    inEdgeList.clear();
    outEdgeList.clear();
    this->hide();
    graphicsWidget->removeItem(this);

 }

