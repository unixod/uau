#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QList>
#include "rangeset.h"

//for data driven testing
typedef uau::RangeSet<int, QList>::value_type Range;
Q_DECLARE_METATYPE(QList<Range>)

class RangesetTest : public QObject{
    Q_OBJECT
private Q_SLOTS:
    void testCase_data();
    void testCase();
};

void RangesetTest::testCase_data(){
    QTest::addColumn<QList<Range>>("exclusions");
    QTest::addColumn<QList<Range>>("result");
    QTest::newRow("back deletion") << QList<Range>{{10, 12}, {6, 7}, {16, 17}, {0, 6}}
                          << QList<Range>{{8, 9}, {13, 15}, {18, 20}};
    QTest::newRow("front deletion") << QList<Range>{{10, 12}, {6, 7}, {16, 17}, {18, 30}}
                      << QList<Range>{{0, 5}, {8, 9}, {13, 15}};
    QTest::newRow("midle deletion") << QList<Range>{{10, 12}, {6, 7}, {16, 17}, {8, 9}}
                          << QList<Range>{{0, 5}, {13, 15}, {18, 20}};
    QTest::newRow("range deletion") << QList<Range>{{10, 12}, {6, 7}, {16, 17}, {3, 14}}
                          << QList<Range>{{0, 2}, {15, 15}, {18, 20}};
}

void RangesetTest::testCase(){
    uau::RangeSet<int, QList> rng;
    rng.assign_range(0, 20);

    QFETCH(QList<Range>, exclusions);
    QFETCH(QList<Range>, result);

    for(auto i: exclusions)
        rng.exclude(i.first, i.second);

    QVERIFY2(result == rng, "Failure");
}

QTEST_APPLESS_MAIN(RangesetTest)

#include "tst_rangeset.moc"
