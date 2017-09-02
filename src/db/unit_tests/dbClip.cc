
/*

  KLayout Layout Viewer
  Copyright (C) 2006-2017 Matthias Koefferlein

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/



#include "dbClip.h"
#include "dbEdgeProcessor.h"
#include "tlUnitTest.h"
#include "tlTimer.h"

TEST(1) 
{
  db::Point input[] = {
    db::Point(0, 0),
    db::Point(0, 100),
    db::Point(100, 100),
    db::Point(100, 80),
    db::Point(20, 80),
    db::Point(20, 20),
    db::Point(80, 20),
    db::Point(80, 80),
    db::Point(100, 80),
    db::Point(100, 0),
  };

  db::SimplePolygon in_poly;
  std::vector<db::SimplePolygon> out_spoly;

  db::Box box (0, 0, 90, 90);

  db::FTrans t270 (db::FTrans::r270);
  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]), t270);
  out_spoly.clear ();
  db::clip_poly (in_poly, t270 * box, out_spoly);

  EXPECT_EQ (out_spoly.size (), size_t (1));
  EXPECT_EQ (out_spoly[0].to_string(), "(0,-90;0,-20;20,-20;20,-80;80,-80;80,-20;0,-20;0,0;90,0;90,-90)");

  db::FTrans t180 (db::FTrans::r180);
  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]), t180);
  out_spoly.clear ();
  db::clip_poly (in_poly, t180 * box, out_spoly);

  EXPECT_EQ (out_spoly.size (), size_t (1));
  EXPECT_EQ (out_spoly[0].to_string(), "(-90,-90;-90,-20;-80,-20;-80,-80;-20,-80;-20,-20;-90,-20;-90,0;0,0;0,-90)");

  db::FTrans t90 (db::FTrans::r90);
  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]), t90);
  out_spoly.clear ();
  db::clip_poly (in_poly, t90 * box, out_spoly);

  EXPECT_EQ (out_spoly.size (), size_t (1));
  EXPECT_EQ (out_spoly[0].to_string(), "(-90,0;-90,80;-80,80;-80,20;-20,20;-20,80;-90,80;-90,90;0,90;0,0)");

  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]));
  out_spoly.clear ();
  db::clip_poly (in_poly, box, out_spoly);

  EXPECT_EQ (out_spoly.size (), size_t (1));
  EXPECT_EQ (out_spoly[0].to_string(), "(0,0;0,80;20,80;20,20;80,20;80,80;0,80;0,90;90,90;90,0)");
}

TEST(2) 
{
  db::Point input[] = {
    db::Point(0,0),
    db::Point(0,1700),
    db::Point(1400,1700),
    db::Point(1400,1000),
    db::Point(2400,1000),
    db::Point(400,1000),
    db::Point(400,500),
    db::Point(1000,500),
    db::Point(1000,1000),
    db::Point(2400,1000),
    db::Point(2400,0),
  };

  db::SimplePolygon in_poly;
  std::vector<db::SimplePolygon> out_spoly;
  db::Box box;
  
  box = db::Box (0, 0, 2200, 2000);

  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]));
  out_spoly.clear ();
  db::clip_poly (in_poly, box, out_spoly);

  EXPECT_EQ (out_spoly.size (), size_t (1));
  EXPECT_EQ (out_spoly[0].to_string(), "(0,0;0,1000;400,1000;400,500;1000,500;1000,1000;0,1000;0,1700;1400,1700;1400,1000;2200,1000;2200,0)");
  
  box = db::Box (0, 0, 1200, 2000);

  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]));
  out_spoly.clear ();
  db::clip_poly (in_poly, box, out_spoly);

  EXPECT_EQ (out_spoly.size (), size_t (1));
  EXPECT_EQ (out_spoly[0].to_string(), "(0,0;0,1000;400,1000;400,500;1000,500;1000,1000;0,1000;0,1700;1200,1700;1200,0)");
  
  box = db::Box (0, 0, 1200, 1200);

  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]));
  out_spoly.clear ();
  db::clip_poly (in_poly, box, out_spoly);

  EXPECT_EQ (out_spoly.size (), size_t (1));
  EXPECT_EQ (out_spoly[0].to_string(), "(0,0;0,1000;400,1000;400,500;1000,500;1000,1000;0,1000;0,1200;1200,1200;1200,0)");
  
  box = db::Box (200, 200, 1200, 1200);

  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]));
  out_spoly.clear ();
  db::clip_poly (in_poly, box, out_spoly);

  EXPECT_EQ (out_spoly.size (), size_t (1));
  EXPECT_EQ (out_spoly[0].to_string(), "(200,200;200,1000;400,1000;400,500;1000,500;1000,1000;200,1000;200,1200;1200,1200;1200,200)");
  
  box = db::Box (200, 200, 800, 1200);

  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]));
  out_spoly.clear ();
  db::clip_poly (in_poly, box, out_spoly);

  EXPECT_EQ (out_spoly.size (), size_t (1));
  EXPECT_EQ (out_spoly[0].to_string(), "(200,200;200,1200;800,1200;800,1000;400,1000;400,500;800,500;800,200)");
}

TEST(3) 
{
  db::Point input[] = {
    db::Point(104931394,20747500),
    db::Point(106766597,22582703),
    db::Point(106807799,22610233),
    db::Point(106856400,22619900),
    db::Point(110033900,22619900),
    db::Point(110045764,22617540),
    db::Point(110050508,22618810),
    db::Point(110056352,22615434),
    db::Point(110082501,22610233),
    db::Point(110105955,22586779),
    db::Point(110134675,22570188),
    db::Point(110146462,22546272),
    db::Point(110151233,22541501),
    db::Point(110151233,22536591),
    db::Point(110156581,22525740),
    db::Point(110204854,22345404),
    db::Point(110349724,22094482),
    db::Point(110554482,21889724),
    db::Point(110805416,21744847),
    db::Point(111085214,21669900),
    db::Point(111374879,21669900),
    db::Point(111654582,21744846),
    db::Point(111905518,21889724),
    db::Point(112110276,22094482),
    db::Point(112255153,22345416),
    db::Point(112330100,22625214),
    db::Point(112330100,22914879),
    db::Point(112255154,23194582),
    db::Point(112110276,23445518),
    db::Point(111905518,23650276),
    db::Point(111693100,23772915),
    db::Point(111655844,23805588),
    db::Point(111651403,23822163),
    db::Point(111639267,23834299),
    db::Point(111639267,23867455),
    db::Point(111630687,23899477),
    db::Point(111639267,23914338),
    db::Point(111639267,23931501),
    db::Point(111662712,23954946),
    db::Point(111679288,23983656),
    db::Point(111695863,23988097),
    db::Point(111707999,24000233),
    db::Point(111756600,24009900),
    db::Point(112682194,24009900),
    db::Point(113030397,24358103),
    db::Point(113040462,24364828),
    db::Point(113042918,24369079),
    db::Point(113049434,24370823),
    db::Point(113071599,24385633),
    db::Point(113104774,24385633),
    db::Point(113136815,24394208),
    db::Point(113162052,24385633),
    db::Point(113168801,24385633),
    db::Point(113172275,24382159),
    db::Point(113183733,24378266),
    db::Point(113345435,24284842),
    db::Point(113625214,24209900),
    db::Point(113914879,24209900),
    db::Point(114194582,24284846),
    db::Point(114445518,24429724),
    db::Point(114650276,24634482),
    db::Point(114795153,24885416),
    db::Point(114870100,25165214),
    db::Point(114870100,25454879),
    db::Point(114795154,25734582),
    db::Point(114650276,25985518),
    db::Point(114445518,26190276),
    db::Point(114194582,26335154),
    db::Point(113914879,26410100),
    db::Point(113625214,26410100),
    db::Point(113345416,26335153),
    db::Point(113094482,26190276),
    db::Point(112889724,25985518),
    db::Point(112744846,25734582),
    db::Point(112669900,25454879),
    db::Point(112669900,25165214),
    db::Point(112744842,24885435),
    db::Point(112838266,24723733),
    db::Point(112842159,24712275),
    db::Point(112845633,24708801),
    db::Point(112845633,24702052),
    db::Point(112854208,24676815),
    db::Point(112845633,24644774),
    db::Point(112845633,24611599),
    db::Point(112830823,24589434),
    db::Point(112829079,24582918),
    db::Point(112824828,24580462),
    db::Point(112818103,24570397),
    db::Point(112595003,24347297),
    db::Point(112553801,24319767),
    db::Point(112505200,24310100),
    db::Point(112092500,24310100),
    db::Point(112043899,24319767),
    db::Point(111975167,24388499),
    db::Point(111975167,24485701),
    db::Point(112002697,24526903),
    db::Point(112110276,24634482),
    db::Point(112255153,24885416),
    db::Point(112330100,25165214),
    db::Point(112330100,25454879),
    db::Point(112255154,25734582),
    db::Point(112110276,25985518),
    db::Point(111905518,26190276),
    db::Point(111654582,26335154),
    db::Point(111374879,26410100),
    db::Point(111085214,26410100),
    db::Point(110805416,26335153),
    db::Point(110554482,26190276),
    db::Point(110349724,25985518),
    db::Point(110204846,25734582),
    db::Point(110129900,25454879),
    db::Point(110129900,25165214),
    db::Point(110204847,24885416),
    db::Point(110349724,24634482),
    db::Point(110457303,24526903),
    db::Point(110484833,24485701),
    db::Point(110484833,24388499),
    db::Point(110416101,24319767),
    db::Point(110367500,24310100),
    db::Point(107936800,24310100),
    db::Point(107888199,24319767),
    db::Point(107846997,24347297),
    db::Point(105644194,26550100),
    db::Point(102977000,26550100),
    db::Point(102928399,26559767),
    db::Point(102859667,26628499),
    db::Point(102850000,26677100),
    db::Point(102850000,27150000),
    db::Point(101100000,27150000),
    db::Point(101100000,26877100),
    db::Point(101090333,26828499),
    db::Point(101021601,26759767),
    db::Point(100973000,26750100),
    db::Point(95027000,26750100),
    db::Point(94978399,26759767),
    db::Point(94909667,26828499),
    db::Point(94900000,26877100),
    db::Point(94900000,27150000),
    db::Point(93150000,27150000),
    db::Point(93150000,26877100),
    db::Point(93140333,26828499),
    db::Point(93071601,26759767),
    db::Point(93023000,26750100),
    db::Point(91977000,26750100),
    db::Point(91928399,26759767),
    db::Point(91859667,26828499),
    db::Point(91850000,26877100),
    db::Point(91850000,27150000),
    db::Point(90100000,27150000),
    db::Point(90100000,26877100),
    db::Point(90090333,26828499),
    db::Point(90021601,26759767),
    db::Point(89973000,26750100),
    db::Point(85000000,26750100),
    db::Point(85000000,28549900),
    db::Point(89973000,28549900),
    db::Point(90021601,28540233),
    db::Point(90090333,28471501),
    db::Point(90100000,28422900),
    db::Point(90100000,28150000),
    db::Point(91850000,28150000),
    db::Point(91850000,29659349),
    db::Point(91848399,29659667),
    db::Point(91779667,29728399),
    db::Point(91770000,29777000),
    db::Point(91770000,29949500),
    db::Point(91771604,29957564),
    db::Point(91770525,29961031),
    db::Point(91775938,29979351),
    db::Point(91779667,29998101),
    db::Point(91782236,30000670),
    db::Point(91784565,30008553),
    db::Point(91800200,30038322),
    db::Point(91800200,30161678),
    db::Point(91784565,30191447),
    db::Point(91782236,30199330),
    db::Point(91779667,30201899),
    db::Point(91775938,30220649),
    db::Point(91770525,30238969),
    db::Point(91771604,30242436),
    db::Point(91770000,30250500),
    db::Point(91770000,30500000),
    db::Point(91015000,30500000),
    db::Point(91015000,29777000),
    db::Point(91005333,29728399),
    db::Point(90936601,29659667),
    db::Point(90888000,29650000),
    db::Point(90100000,29650000),
    db::Point(90100000,29377100),
    db::Point(90090333,29328499),
    db::Point(90021601,29259767),
    db::Point(89973000,29250100),
    db::Point(85000000,29250100),
    db::Point(85000000,33049900),
    db::Point(89973000,33049900),
    db::Point(90021601,33040233),
    db::Point(90090333,32971501),
    db::Point(90100000,32922900),
    db::Point(90100000,32650000),
    db::Point(91850000,32650000),
    db::Point(91850000,32922900),
    db::Point(91859667,32971501),
    db::Point(91928399,33040233),
    db::Point(91977000,33049900),
    db::Point(93023000,33049900),
    db::Point(93071601,33040233),
    db::Point(93140333,32971501),
    db::Point(93150000,32922900),
    db::Point(93150000,32650000),
    db::Point(94900000,32650000),
    db::Point(94900000,32922900),
    db::Point(94909667,32971501),
    db::Point(94978399,33040233),
    db::Point(95027000,33049900),
    db::Point(100973000,33049900),
    db::Point(101021601,33040233),
    db::Point(101090333,32971501),
    db::Point(101100000,32922900),
    db::Point(101100000,32650000),
    db::Point(102850000,32650000),
    db::Point(102850000,34150000),
    db::Point(101100000,34150000),
    db::Point(101100000,33877100),
    db::Point(101090333,33828499),
    db::Point(101021601,33759767),
    db::Point(100973000,33750100),
    db::Point(95027000,33750100),
    db::Point(94978399,33759767),
    db::Point(94909667,33828499),
    db::Point(94900000,33877100),
    db::Point(94900000,34150000),
    db::Point(93150000,34150000),
    db::Point(93150000,33877100),
    db::Point(93140333,33828499),
    db::Point(93071601,33759767),
    db::Point(93023000,33750100),
    db::Point(91977000,33750100),
    db::Point(91928399,33759767),
    db::Point(91859667,33828499),
    db::Point(91850000,33877100),
    db::Point(91850000,34150000),
    db::Point(90100000,34150000),
    db::Point(90100000,33877100),
    db::Point(90090333,33828499),
    db::Point(90021601,33759767),
    db::Point(89973000,33750100),
    db::Point(85000000,33750100),
    db::Point(85000000,35749900),
    db::Point(91790500,35749900),
    db::Point(91839091,35740235),
    db::Point(91839096,35740235),
    db::Point(91839098,35740234),
    db::Point(91839101,35740233),
    db::Point(91839105,35740229),
    db::Point(91880299,35712706),
    db::Point(92512956,35080100),
    db::Point(92693071,34900000),
    db::Point(93066802,34526300),
    db::Point(95562500,34526300),
    db::Point(95611101,34516633),
    db::Point(95614047,34513687),
    db::Point(95618207,34513431),
    db::Point(95649786,34489402),
    db::Point(95768951,34444245),
    db::Point(95891341,34459104),
    db::Point(95992907,34529159),
    db::Point(96050200,34638307),
    db::Point(96050200,34761693),
    db::Point(95992907,34870841),
    db::Point(95891341,34940896),
    db::Point(95768951,34955755),
    db::Point(95653625,34912052),
    db::Point(95636706,34892934),
    db::Point(95597258,34862945),
    db::Point(95593127,34862693),
    db::Point(95590201,34859767),
    db::Point(95541600,34850100),
    db::Point(95431053,34850100),
    db::Point(95410201,34836167),
    db::Point(95361600,34826500),
    db::Point(93243800,34826500),
    db::Point(93195199,34836167),
    db::Point(93153997,34863697),
    db::Point(93084497,34933197),
    db::Point(93056967,34974399),
    db::Point(93056967,35056967),
    db::Point(93013286,35056967),
    db::Point(92974405,35056965),
    db::Point(92974402,35056967),
    db::Point(92974399,35056967),
    db::Point(92974393,35056973),
    db::Point(92933202,35084493),
    db::Point(91967499,36050100),
    db::Point(85000000,36050100),
    db::Point(85000000,45150000),
    db::Point(127404015,45150000),
    db::Point(127404015,38300000),
    db::Point(126050000,38300000),
    db::Point(126050000,37700000),
    db::Point(126150000,37700000),
    db::Point(126150000,38200000),
    db::Point(126775000,38200000),
    db::Point(126775000,38300000),
    db::Point(127075000,38300000),
    db::Point(127075000,38200000),
    db::Point(127404015,38200000),
    db::Point(127404015,36900000),
    db::Point(127075000,36900000),
    db::Point(127075000,36800000),
    db::Point(127404015,36800000),
    db::Point(127404015,33800000),
    db::Point(126050000,33800000),
    db::Point(126050000,32300000),
    db::Point(127404015,32300000),
    db::Point(127404015,20747500)
  };

  db::SimplePolygon in_poly;
  in_poly.assign_hull(input, input + sizeof(input) / sizeof(input[0]));

  db::Box box (db::Point(85000000,20747500),db::Point(105644194,45150000));

  std::vector<db::Polygon> out_poly;
  std::vector<db::SimplePolygon> out_spoly;
  
  {
    tl::SelfTimer timer("clip");
    for (unsigned int i = 0; i < 10000; ++i) {  
      out_spoly.clear();
      db::clip_poly (in_poly, box, out_spoly);
    }
  }

  EXPECT_EQ (out_spoly.size (), size_t (2));
  EXPECT_EQ (out_spoly[0].to_string (), "(104931394,20747500;105644194,21460300;105644194,20747500)");
  EXPECT_EQ (out_spoly[1].to_string (), "(102977000,26550100;102928399,26559767;102859667,26628499;102850000,26677100;"
                                         "102850000,27150000;101100000,27150000;101100000,26877100;101090333,26828499;"
                                         "101021601,26759767;100973000,26750100;95027000,26750100;94978399,26759767;"
                                         "94909667,26828499;94900000,26877100;94900000,27150000;93150000,27150000;"
                                         "93150000,26877100;93140333,26828499;93071601,26759767;93023000,26750100;"
                                         "91977000,26750100;91928399,26759767;91859667,26828499;91850000,26877100;"
                                         "91850000,27150000;90100000,27150000;90100000,26877100;90090333,26828499;"
                                         "90021601,26759767;89973000,26750100;85000000,26750100;85000000,28549900;"
                                         "89973000,28549900;90021601,28540233;90090333,28471501;90100000,28422900;"
                                         "90100000,28150000;91850000,28150000;91850000,29659349;91848399,29659667;"
                                         "91779667,29728399;91770000,29777000;91770000,29949500;91771604,29957564;"
                                         "91770525,29961031;91775938,29979351;91779667,29998101;91782236,30000670;"
                                         "91784565,30008553;91800200,30038322;91800200,30161678;91784565,30191447;"
                                         "91782236,30199330;91779667,30201899;91775938,30220649;91770525,30238969;"
                                         "91771604,30242436;91770000,30250500;91770000,30500000;91015000,30500000;"
                                         "91015000,29777000;91005333,29728399;90936601,29659667;90888000,29650000;"
                                         "90100000,29650000;90100000,29377100;90090333,29328499;90021601,29259767;"
                                         "89973000,29250100;85000000,29250100;85000000,33049900;89973000,33049900;"
                                         "90021601,33040233;90090333,32971501;90100000,32922900;90100000,32650000;"
                                         "91850000,32650000;91850000,32922900;91859667,32971501;91928399,33040233;"
                                         "91977000,33049900;93023000,33049900;93071601,33040233;93140333,32971501;"
                                         "93150000,32922900;93150000,32650000;94900000,32650000;94900000,32922900;"
                                         "94909667,32971501;94978399,33040233;95027000,33049900;100973000,33049900;"
                                         "101021601,33040233;101090333,32971501;101100000,32922900;101100000,32650000;"
                                         "102850000,32650000;102850000,34150000;101100000,34150000;101100000,33877100;"
                                         "101090333,33828499;101021601,33759767;100973000,33750100;95027000,33750100;"
                                         "94978399,33759767;94909667,33828499;94900000,33877100;94900000,34150000;"
                                         "93150000,34150000;93150000,33877100;93140333,33828499;93071601,33759767;"
                                         "93023000,33750100;91977000,33750100;91928399,33759767;91859667,33828499;"
                                         "91850000,33877100;91850000,34150000;90100000,34150000;90100000,33877100;"
                                         "90090333,33828499;90021601,33759767;89973000,33750100;85000000,33750100;"
                                         "85000000,35749900;91790500,35749900;91839091,35740235;91839096,35740235;"
                                         "91839098,35740234;91839101,35740233;91839105,35740229;91880299,35712706;"
                                         "92512956,35080100;92693071,34900000;93066802,34526300;95562500,34526300;"
                                         "95611101,34516633;95614047,34513687;95618207,34513431;95649786,34489402;"
                                         "95768951,34444245;95891341,34459104;95992907,34529159;96050200,34638307;"
                                         "96050200,34761693;95992907,34870841;95891341,34940896;95768951,34955755;"
                                         "95653625,34912052;95636706,34892934;95597258,34862945;95593127,34862693;"
                                         "95590201,34859767;95541600,34850100;95431053,34850100;95410201,34836167;"
                                         "95361600,34826500;93243800,34826500;93195199,34836167;93153997,34863697;"
                                         "93084497,34933197;93056967,34974399;93056967,35056967;93013286,35056967;"
                                         "92974405,35056965;92974402,35056967;92974399,35056967;92974393,35056973;"
                                         "92933202,35084493;91967499,36050100;85000000,36050100;85000000,45150000;"
                                         "105644194,45150000;105644194,26550100)");
}

TEST(4) 
{
  db::Point hull[] = {
    db::Point (1600, 10),
    db::Point (10,   200),
    db::Point (10,   210),
    db::Point (0,    220),
    db::Point (70,   220),
    db::Point (1700, 30),
    db::Point (1700, 20),
    db::Point (1650, 10)
  };

  db::Polygon in_poly;
  in_poly.assign_hull(hull, hull + sizeof(hull) / sizeof(hull[0]));

  db::Box box (db::Point(20,0),db::Point(100,100));

  std::vector<db::Polygon> out_poly;
  db::clip_poly (in_poly, box, out_poly);
  EXPECT_EQ (out_poly.size (), size_t (0));
}

TEST(5) 
{
  db::Point hull[] = {
    db::Point ( 54,0),
    db::Point ( 54,10),
    db::Point (123,1001),
    db::Point (126,1003),
    db::Point (126,1004),
    db::Point (122,1020),
    db::Point (125,1020),
    db::Point (121,1006),
    db::Point ( 53,50),
    db::Point ( 52,30),
    db::Point ( 51,20),
    db::Point ( 51,40),
    db::Point (120,1021),
    db::Point (124,1021),
    db::Point (127,1005),
    db::Point (127,1002)
  };

  db::Polygon in_poly;
  in_poly.assign_hull(hull, hull + sizeof(hull) / sizeof(hull[0]));

  db::Box box (db::Point(0,0),db::Point(100,1010));

  std::vector<db::Polygon> out_poly;
  db::clip_poly (in_poly, box, out_poly);
  EXPECT_EQ (out_poly.size (), size_t (2));

  EXPECT_EQ (out_poly[0].to_string(), "(54,0;54,10;100,671;100,631)");
  EXPECT_EQ (out_poly[1].to_string(), "(51,20;51,40;100,737;100,711;53,50;52,30)");
}



