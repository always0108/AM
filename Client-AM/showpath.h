#ifndef SHOWPATH_H
#define SHOWPATH_H

#include <QWidget>
#include <iostream>
#include <fstream>
#include <QVector>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_legenditem.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_textlabel.h>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_renderer.h>
#include <QDebug>

typedef signed long long cInt;
struct IntPoint {
  cInt X;
  cInt Y;
#ifdef use_xyz
  cInt Z;
  IntPoint(cInt x = 0, cInt y = 0, cInt z = 0): X(x), Y(y), Z(z) {};
#else
  IntPoint(cInt x = 0, cInt y = 0): X(x), Y(y) {};
#endif

  friend inline bool operator== (const IntPoint& a, const IntPoint& b)
  {
    return a.X == b.X && a.Y == b.Y;
  }
  friend inline bool operator!= (const IntPoint& a, const IntPoint& b)
  {
    return a.X != b.X  || a.Y != b.Y;
  }
};
typedef IntPoint Point;
typedef std::vector< IntPoint > Path;
typedef std::vector< Path > Paths;
typedef Paths Polygons;
typedef std::vector< Polygons > Layers;

class Showpath : public QWidget
{
    Q_OBJECT
public:
    explicit Showpath(QWidget *parent = nullptr);

    QwtPlot *qwtPlot;
    std::ifstream infile;
    Polygons polygons;
    Layers layer;
    Layers layer_path;
    void plotlayer(QString input);
    void plotpath(QString input);
    void clearpath();
    QVector<QwtPlotCurve*> curve_sum1;
    QVector<QwtPlotCurve*> curve_sum2;

signals:

};

#endif // SHOWPATH_H
