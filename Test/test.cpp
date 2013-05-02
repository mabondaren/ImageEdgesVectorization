#include <QString>
#include <QtTest>

#define private public

#include "contouralgorithms.h"

namespace
{
    bool operator ==(const Contour& i_one, const Contour& i_second)
    {
        if(i_one.IsClosed() && i_second.IsClosed() && i_one.GetContourPoints().size() ==
                i_second.GetContourPoints().size())
        {
            for(int i = 0; i<i_one.GetContourPoints().size(); ++i)
                if(i_one.GetContourPoints().indexOf(i_second.GetContourPoints()[i]) == -1)
                    return false;

            return true;
        }

        return i_one.GetContourPoints() == i_second.GetContourPoints() && i_one.IsClosed() == i_second.IsClosed();
    }
}

class Test : public QObject
{
    Q_OBJECT
    
public:
    Test();
    
private Q_SLOTS:
    void LinesToContoursTest();
    void LinesToContoursTest_data();

    void LinesToTLinkedPointsTest();
    void LinesToTLinkedPointsTest_data();

    void TLinkedPointsToContoursTest();
    void TLinkedPointsToContoursTest_data();

    void CombineContourTest();
    void CombineContourTest_data();
};

Test::Test()
{
}


void Test::LinesToContoursTest()
{
    QFETCH(Lines, lines);
    QFETCH(TContours, result);

    QCOMPARE(ContourAlgorithms::LinesToContours(lines), result);
}

void Test::LinesToContoursTest_data()
{
    QTest::addColumn<Lines>("lines");
    QTest::addColumn<TContours>("result");

    Lines lines;
    lines.m_lines.push_back(QLine(0,0,0,1));
    lines.m_lines.push_back(QLine(0,1,1,1));
    lines.m_lines.push_back(QLine(1,1,1,0));
    lines.m_lines.push_back(QLine(1,0,0,0));

    TContours contours;
    Contour contour;
    contour.AddPoint(QPoint(0, 0));
    contour.AddPoint(QPoint(0, 1));
    contour.AddPoint(QPoint(1, 1));
    contour.AddPoint(QPoint(1, 0));
    contour.MakeClosed();
    contours.push_back(contour);

    QTest::newRow("1")<<lines<<contours;
}

void Test::LinesToTLinkedPointsTest()
{
    QFETCH(Lines, lines);
    QFETCH(TLinkedPoints, result);

    QCOMPARE(ContourAlgorithms::_LinesToLinkedPoints(lines), result);
}

void Test::LinesToTLinkedPointsTest_data()
{
    QTest::addColumn<Lines>("lines");
    QTest::addColumn<TLinkedPoints>("result");

    Lines lines;
    lines.m_lines.push_back(QLine(0,0,0,1));
    lines.m_lines.push_back(QLine(0,1,1,1));
    lines.m_lines.push_back(QLine(1,1,1,0));
    lines.m_lines.push_back(QLine(1,0,0,0));

    TLinkedPoints linked_points;
    linked_points.insert(QPoint(0, 0), qMakePair(QPoint(1, 0), QPoint(0, 1)));
    linked_points.insert(QPoint(0, 1), qMakePair(QPoint(0, 0), QPoint(1, 1)));
    linked_points.insert(QPoint(1, 1), qMakePair(QPoint(0, 1), QPoint(1, 0)));
    linked_points.insert(QPoint(1, 0), qMakePair(QPoint(1, 1), QPoint(0, 0)));

    QTest::newRow("1")<<lines<<linked_points;
}

void Test::TLinkedPointsToContoursTest()
{
    QFETCH(TLinkedPoints, linked_points);
    QFETCH(TContours, result);

    QCOMPARE(ContourAlgorithms::_LinkedPointsToContours(linked_points), result);
}

void Test::TLinkedPointsToContoursTest_data()
{
    QTest::addColumn<TLinkedPoints>("linked_points");
    QTest::addColumn<TContours>("result");

    TLinkedPoints linked_points;
    linked_points.insert(QPoint(0, 0), qMakePair(QPoint(1, 0), QPoint(0, 1)));
    linked_points.insert(QPoint(0, 1), qMakePair(QPoint(0, 0), QPoint(1, 1)));
    linked_points.insert(QPoint(1, 1), qMakePair(QPoint(0, 1), QPoint(1, 0)));
    linked_points.insert(QPoint(1, 0), qMakePair(QPoint(1, 1), QPoint(0, 0)));

    TContours contours;
    Contour contour;
    contour.AddPoint(QPoint(0, 0));
    contour.AddPoint(QPoint(0, 1));
    contour.AddPoint(QPoint(1, 1));
    contour.AddPoint(QPoint(1, 0));
    contour.MakeClosed();
    contours.push_back(contour);

    QTest::newRow("1")<<linked_points<<contours;

    linked_points.insert(QPoint(2, 0), qMakePair(QPoint(3, 0), QPoint(2, 1)));
    linked_points.insert(QPoint(2, 1), qMakePair(QPoint(2, 0), QPoint(3, 1)));
    linked_points.insert(QPoint(3, 1), qMakePair(QPoint(2, 1), QPoint(3, 0)));
    linked_points.insert(QPoint(3, 0), qMakePair(QPoint(3, 1), QPoint(2, 0)));

    contour.Clear();
    contour.AddPoint(QPoint(2, 0));
    contour.AddPoint(QPoint(2, 1));
    contour.AddPoint(QPoint(3, 1));
    contour.AddPoint(QPoint(3, 0));
    contour.MakeClosed();
    contours.push_back(contour);

    QTest::newRow("2")<<linked_points<<contours;
}

void Test::CombineContourTest()
{
    QFETCH(Contour, contour);
    QFETCH(Contour, result);

    ContourAlgorithms::_CombineLinesInContour(contour);
    QCOMPARE(contour, result);
}

void Test::CombineContourTest_data()
{
    QTest::addColumn<Contour>("contour");
    QTest::addColumn<Contour>("result");

    Contour contour;
    contour.AddPoint(QPoint(0, 0));
    contour.AddPoint(QPoint(1, 0));
    contour.AddPoint(QPoint(2, 0));

    Contour result;
    result.AddPoint(QPoint(0, 0));
    result.AddPoint(QPoint(2, 0));

    QTest::newRow("1")<<contour<<result;

    contour.Clear();
    contour.AddPoint(QPoint(0, 0));
    contour.AddPoint(QPoint(0, 1));
    contour.AddPoint(QPoint(1, 1));
    contour.AddPoint(QPoint(1, 0));

    QTest::newRow("2")<<contour<<contour;
    contour.MakeClosed();

    QTest::newRow("3")<<contour<<contour;

    contour.Clear();
    contour.AddPoint(QPoint(0, 0));
    contour.AddPoint(QPoint(0, 1));
    contour.AddPoint(QPoint(0, 2));
    contour.AddPoint(QPoint(1, 2));
    contour.AddPoint(QPoint(2, 2));
    contour.AddPoint(QPoint(2, 1));
    contour.AddPoint(QPoint(2, 0));
    contour.AddPoint(QPoint(1, 0));
    contour.MakeClosed();

    result.Clear();
    result.AddPoint(QPoint(0, 0));
    result.AddPoint(QPoint(0, 2));
    result.AddPoint(QPoint(2, 2));
    result.AddPoint(QPoint(2, 0));
    result.MakeClosed();

    QTest::newRow("4")<<contour<<result;

    contour.MakeUnclosed();
    result.MakeUnclosed();
    result.AddPoint(QPoint(1, 0));

    QTest::newRow("5")<<contour<<result;

    contour.Clear();
    contour.AddPoint(QPoint(0, 0));
    contour.AddPoint(QPoint(0, 1));
    contour.AddPoint(QPoint(1, 2));
    contour.AddPoint(QPoint(2, 3));
    contour.AddPoint(QPoint(2, 2));
    contour.AddPoint(QPoint(2, 1));
    contour.AddPoint(QPoint(2, 0));
    contour.AddPoint(QPoint(1, 0));
    contour.MakeClosed();

    result.Clear();
    result.AddPoint(QPoint(0, 0));
    result.AddPoint(QPoint(0, 1));
    result.AddPoint(QPoint(2, 3));
    result.AddPoint(QPoint(2, 0));
    result.MakeClosed();

    QTest::newRow("6")<<contour<<result;
    }

#undef public

QTEST_APPLESS_MAIN(Test)

#include "test.moc"
