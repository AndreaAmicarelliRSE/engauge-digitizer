#include "CmdMediator.h"
#include "DocumentModelCoords.h"
#include "DocumentSerialize.h"
#include "EngaugeAssert.h"
#include "Logger.h"
#include <QTextStream>
#include "QtToString.h"
#include <QXmlStreamWriter>
#include "Xml.h"

const double PI = 3.1415926535;
const double TWO_PI = 2.0 * PI;

// Zero default for origin radius, which is usually wanted for COORD_SCALE_LINEAR, it is illegal
// for COORD_SCALE_LOG (for which 1.0 is probably preferred as the default. Note linear is more common than log
const double DEFAULT_ORIGIN_RADIUS_LINEAR = 0.0;

DocumentModelCoords::DocumentModelCoords() :
  m_coordsType (COORDS_TYPE_CARTESIAN),
  m_originRadius (DEFAULT_ORIGIN_RADIUS_LINEAR),
  m_coordScaleXTheta (COORD_SCALE_LINEAR),
  m_coordScaleYRadius (COORD_SCALE_LINEAR),
  m_coordUnitsX (COORD_UNITS_NON_POLAR_THETA_NUMBER),
  m_coordUnitsY (COORD_UNITS_NON_POLAR_THETA_NUMBER),
  m_coordUnitsTheta (COORD_UNITS_POLAR_THETA_DEGREES),
  m_coordUnitsRadius (COORD_UNITS_NON_POLAR_THETA_NUMBER)
{
}

DocumentModelCoords::DocumentModelCoords(const Document &document) :
  m_coordsType (document.modelCoords().coordsType()),
  m_originRadius(document.modelCoords().originRadius()),
  m_coordScaleXTheta(document.modelCoords().coordScaleXTheta()),
  m_coordScaleYRadius(document.modelCoords().coordScaleYRadius()),
  m_coordUnitsX(document.modelCoords().coordUnitsX()),
  m_coordUnitsY(document.modelCoords().coordUnitsY()),
  m_coordUnitsTheta(document.modelCoords().coordUnitsTheta()),
  m_coordUnitsRadius(document.modelCoords().coordUnitsRadius())
{
}

DocumentModelCoords::DocumentModelCoords(const DocumentModelCoords &other) :
  m_coordsType (other.coordsType ()),
  m_originRadius (other.originRadius ()),
  m_coordScaleXTheta (other.coordScaleXTheta()),
  m_coordScaleYRadius (other.coordScaleYRadius ()),
  m_coordUnitsX (other.coordUnitsX()),
  m_coordUnitsY (other.coordUnitsY()),
  m_coordUnitsTheta (other.coordUnitsTheta ()),
  m_coordUnitsRadius (other.coordUnitsRadius ())
{
}

DocumentModelCoords &DocumentModelCoords::operator=(const DocumentModelCoords &other)
{
  m_coordsType = other.coordsType();
  m_originRadius = other.originRadius();
  m_coordScaleXTheta = other.coordScaleXTheta();
  m_coordScaleYRadius = other.coordScaleYRadius();
  m_coordUnitsX = other.coordUnitsX();
  m_coordUnitsY = other.coordUnitsY();
  m_coordUnitsTheta = other.coordUnitsTheta();
  m_coordUnitsRadius = other.coordUnitsRadius();

  return *this;
}

CoordScale DocumentModelCoords::coordScaleXTheta () const
{
  return m_coordScaleXTheta;
}

CoordScale DocumentModelCoords::coordScaleYRadius () const
{
  return m_coordScaleYRadius;
}

CoordsType DocumentModelCoords::coordsType () const
{
  return m_coordsType;
}

CoordUnitsNonPolarTheta DocumentModelCoords::coordUnitsRadius() const
{
  return m_coordUnitsRadius;
}

CoordUnitsPolarTheta DocumentModelCoords::coordUnitsTheta() const
{
  return m_coordUnitsTheta;
}

CoordUnitsNonPolarTheta DocumentModelCoords::coordUnitsX() const
{
  return m_coordUnitsX;
}

CoordUnitsNonPolarTheta DocumentModelCoords::coordUnitsY() const
{
  return m_coordUnitsY;
}

void DocumentModelCoords::loadXml(QXmlStreamReader &reader)
{
  LOG4CPP_INFO_S ((*mainCat)) << "DocumentModelCoords::loadXml";

  bool success = true;

  QXmlStreamAttributes attributes = reader.attributes();

  if (attributes.hasAttribute(DOCUMENT_SERIALIZE_COORDS_TYPE) &&
      attributes.hasAttribute(DOCUMENT_SERIALIZE_COORDS_ORIGIN_RADIUS) &&
      attributes.hasAttribute(DOCUMENT_SERIALIZE_COORDS_SCALE_X_THETA) &&
      attributes.hasAttribute(DOCUMENT_SERIALIZE_COORDS_SCALE_Y_RADIUS) &&
      attributes.hasAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_X) &&
      attributes.hasAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_Y) &&
      attributes.hasAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_THETA) &&
      attributes.hasAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_RADIUS)) {

    setCoordsType ((CoordsType) attributes.value(DOCUMENT_SERIALIZE_COORDS_TYPE).toInt());
    setOriginRadius (attributes.value(DOCUMENT_SERIALIZE_COORDS_ORIGIN_RADIUS).toDouble());
    setCoordScaleXTheta ((CoordScale) attributes.value(DOCUMENT_SERIALIZE_COORDS_SCALE_X_THETA).toInt());
    setCoordScaleYRadius ((CoordScale) attributes.value(DOCUMENT_SERIALIZE_COORDS_SCALE_Y_RADIUS).toInt());
    setCoordUnitsX ((CoordUnitsNonPolarTheta) attributes.value(DOCUMENT_SERIALIZE_COORDS_UNITS_X).toInt());
    setCoordUnitsY ((CoordUnitsNonPolarTheta) attributes.value(DOCUMENT_SERIALIZE_COORDS_UNITS_Y).toInt());
    setCoordUnitsTheta ((CoordUnitsPolarTheta) attributes.value(DOCUMENT_SERIALIZE_COORDS_UNITS_THETA).toInt());
    setCoordUnitsRadius ((CoordUnitsNonPolarTheta) attributes.value(DOCUMENT_SERIALIZE_COORDS_UNITS_RADIUS).toInt());

    // Read until end of this subtree
    while ((reader.tokenType() != QXmlStreamReader::EndElement) ||
    (reader.name() != DOCUMENT_SERIALIZE_COORDS)){
      loadNextFromReader(reader);
      if (reader.atEnd()) {
        success = false;
        break;
      }
    }
  }

  if (!success) {
    reader.raiseError ("Cannot read coordinates data");
  }
}

double DocumentModelCoords::originRadius() const
{
  return m_originRadius;
}

void DocumentModelCoords::printStream(QString indentation,
                                      QTextStream &str) const
{
  str << indentation << "DocumentModelCoords\n";

  indentation += INDENTATION_DELTA;

  str << indentation << "coordsType=" << coordsTypeToString (m_coordsType) << "\n";
  str << indentation << "originRadius" << m_originRadius << "\n";
  str << indentation << "coordScaleXTheta=" << coordScaleToString (m_coordScaleXTheta) << "\n";
  str << indentation << "coordScaleYRadius=" << coordScaleToString (m_coordScaleYRadius) << "\n";
  str << indentation << "coordUnitsX=" << coordUnitsNonPolarThetaToString (m_coordUnitsX) << "\n";
  str << indentation << "coordUnitsY=" << coordUnitsNonPolarThetaToString (m_coordUnitsY) << "\n";
  str << indentation << "coordUnitsTheta=" << coordUnitsPolarThetaToString (m_coordUnitsTheta) << "\n";
  str << indentation << "coordUnitsRadius=" << coordUnitsNonPolarThetaToString (m_coordUnitsRadius) << "\n";
}

void DocumentModelCoords::saveXml(QXmlStreamWriter &writer) const
{
  LOG4CPP_INFO_S ((*mainCat)) << "DocumentModelCoords::saveXml";

  writer.writeStartElement(DOCUMENT_SERIALIZE_COORDS);
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_TYPE, QString::number (m_coordsType));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_TYPE_STRING, coordsTypeToString (m_coordsType));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_ORIGIN_RADIUS, QString::number (m_originRadius));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_SCALE_X_THETA, QString::number (m_coordScaleXTheta));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_SCALE_X_THETA_STRING, coordScaleToString (m_coordScaleXTheta));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_SCALE_Y_RADIUS, QString::number (m_coordScaleYRadius));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_SCALE_Y_RADIUS_STRING, coordScaleToString (m_coordScaleYRadius));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_X, QString::number (m_coordUnitsX));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_X_STRING, coordUnitsNonPolarThetaToString (m_coordUnitsX));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_Y, QString::number (m_coordUnitsY));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_Y_STRING, coordUnitsNonPolarThetaToString (m_coordUnitsY));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_THETA, QString::number (m_coordUnitsTheta));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_THETA_STRING, coordUnitsPolarThetaToString (m_coordUnitsTheta));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_RADIUS, QString::number (m_coordUnitsRadius));
  writer.writeAttribute(DOCUMENT_SERIALIZE_COORDS_UNITS_RADIUS_STRING, coordUnitsNonPolarThetaToString (m_coordUnitsRadius));
  writer.writeEndElement();
}

void DocumentModelCoords::setCoordScaleXTheta (CoordScale coordScale)
{
  m_coordScaleXTheta = coordScale;
}

void DocumentModelCoords::setCoordScaleYRadius (CoordScale coordScale)
{
  m_coordScaleYRadius = coordScale;
}

void DocumentModelCoords::setCoordsType (CoordsType coordsType)
{
  m_coordsType = coordsType;
}

void DocumentModelCoords::setCoordUnitsRadius (CoordUnitsNonPolarTheta coordUnitsRadius)
{
  m_coordUnitsRadius = coordUnitsRadius;
}

void DocumentModelCoords::setCoordUnitsTheta (CoordUnitsPolarTheta coordUnitsTheta)
{
  m_coordUnitsTheta = coordUnitsTheta;
}

void DocumentModelCoords::setCoordUnitsX (CoordUnitsNonPolarTheta coordUnitsX)
{
  m_coordUnitsX = coordUnitsX;
}

void DocumentModelCoords::setCoordUnitsY (CoordUnitsNonPolarTheta coordUnitsY)
{
  m_coordUnitsY = coordUnitsY;
}

void DocumentModelCoords::setOriginRadius(double originRadius)
{
  m_originRadius = originRadius;
}

double DocumentModelCoords::thetaPeriod () const
{
  switch (m_coordUnitsTheta) {
    case COORD_UNITS_POLAR_THETA_DEGREES:
    case COORD_UNITS_POLAR_THETA_DEGREES_MINUTES:
    case COORD_UNITS_POLAR_THETA_DEGREES_MINUTES_SECONDS:
      return 360;

    case COORD_UNITS_POLAR_THETA_GRADIANS:
      return 400;

    case COORD_UNITS_POLAR_THETA_RADIANS:
      return TWO_PI;

    case COORD_UNITS_POLAR_THETA_TURNS:
      return 1;
  }

  ENGAUGE_ASSERT(false);
  return 0;
}
