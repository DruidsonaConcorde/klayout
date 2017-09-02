
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


#include "dbPath.h"
#include "tlVector.h"
#include "tlReuseVector.h"
#include "tlUnitTest.h"

#include <vector>

std::string to_string (const tl::vector<db::Point> &pts)
{
  std::string s = "(";

  //  the hull contour
  for (tl::vector<db::Point>::const_iterator p = pts.begin (); p != pts.end (); ++p) {
    if (p != pts.begin ()) {
      s += ";";
    }
    s += p->to_string ();
  }

  s += ")";

  return s;
}

TEST(1) 
{
  db::Path p;
  db::Path empty;
  db::Box b;

  EXPECT_EQ (empty == p, true);

  std::vector <db::Point> c1;
  c1.push_back (db::Point (0, 0));
  c1.push_back (db::Point (0, 1000));
  p.assign (c1.begin (), c1.end ());
  p.width (200);
  b = p.box ();
  //  simple: EXPECT_EQ (b, db::Box (-200, -200, 200, 1200));
  EXPECT_EQ (b, db::Box (-100, 0, 100, 1000));

  EXPECT_EQ (p.to_string (), "(0,0;0,1000) w=200 bx=0 ex=0 r=false");

  db::DPath dp (p, db::cast_op<db::DPoint, db::Point> (), db::cast_op<db::Coord, double> ());
  EXPECT_EQ (dp.to_string (), "(0,0;0,1000) w=200 bx=0 ex=0 r=false");

  db::Path ip = db::Path (dp);
  EXPECT_EQ (ip.to_string (), "(0,0;0,1000) w=200 bx=0 ex=0 r=false");

  db::Path::pointlist_type pts;
  p.hull (pts);
  EXPECT_EQ (to_string (pts), "(-100,0;-100,1000;100,1000;100,0)");

  //  asymmetric width
  pts.clear ();
  p.hull (pts, 50, 150);
  EXPECT_EQ (to_string (pts), "(-50,0;-50,1000;150,1000;150,0)");

  pts.clear ();
  p.hull (pts, 50, 0);
  EXPECT_EQ (to_string (pts), "(-50,0;-50,1000;0,1000;0,0)");

  pts.clear ();
  p.hull (pts, -50, 150);
  EXPECT_EQ (to_string (pts), "(50,0;50,1000;150,1000;150,0)");
}

TEST(2) 
{
  db::Path p;
  db::Box b;

  std::vector <db::Point> c1;
  c1.push_back (db::Point (0, 0));
  c1.push_back (db::Point (0, 1000));
  c1.push_back (db::Point (10, 1000));
  p.assign (c1.begin (), c1.end ());
  p.width (200);
  b = p.box ();
  EXPECT_EQ (b, db::Box (-100, 0, 100, 1100));

  EXPECT_EQ (p.to_string (), "(0,0;0,1000;10,1000) w=200 bx=0 ex=0 r=false");

  db::Path::pointlist_type pts;
  p.hull (pts);
  EXPECT_EQ (to_string (pts), "(-100,0;-100,1100;10,1100;10,900;100,900;100,0)");
}

TEST(2a) 
{
  db::Path p;
  db::Box b;

  std::vector <db::Point> c1;
  c1.push_back (db::Point (0, 0));
  c1.push_back (db::Point (0, 1000));
  c1.push_back (db::Point (1000, 1000));
  p.assign (c1.begin (), c1.end ());
  p.width (200);

  db::Path::pointlist_type pts;
  p.hull (pts, 100, -50);
  EXPECT_EQ (to_string (pts), "(-100,0;-100,1100;1000,1100;1000,1050;-50,1050;-50,0)");

  pts.clear ();
  p.hull (pts, 100, 50);
  EXPECT_EQ (to_string (pts), "(-100,0;-100,1100;1000,1100;1000,950;50,950;50,0)");

  pts.clear ();
  p.hull (pts, -50, 100);
  EXPECT_EQ (to_string (pts), "(50,0;50,950;1000,950;1000,900;100,900;100,0)");
}


TEST(3) 
{
  db::Path p;
  db::Box b;


  std::vector <db::Point> c1;
  c1.push_back (db::Point (0, 0));
  c1.push_back (db::Point (0, 1000));
  c1.push_back (db::Point (1000, 0));
  p.assign (c1.begin (), c1.end ());
  p.width (200);
  b = p.box ();
  //  simple: EXPECT_EQ (b, db::Box (-200, -200, 1200, 1200));
  EXPECT_EQ (b, db::Box (-100, -71, 1071, 1142));

  EXPECT_EQ (p.to_string (), "(0,0;0,1000;1000,0) w=200 bx=0 ex=0 r=false");

  db::Path::pointlist_type pts;
  p.hull (pts);
  EXPECT_EQ (to_string (pts), "(-100,0;-100,1100;0,1142;1071,71;929,-71;100,758;100,0)");
}

TEST(4) 
{
  db::Path p;

  std::vector <db::Point> c1;
  c1.push_back (db::Point (0, 0));
  c1.push_back (db::Point (0, 1000));
  c1.push_back (db::Point (1000, 2000));
  p.assign (c1.begin (), c1.end ());
  p.width (200);

  EXPECT_EQ (p.to_string (), "(0,0;0,1000;1000,2000) w=200 bx=0 ex=0 r=false");

  db::Path::pointlist_type pts;
  p.hull (pts);
  EXPECT_EQ (to_string (pts), "(-100,0;-100,1042;929,2071;1071,1929;100,958;100,0)");

  db::Box bx;
  for (db::Path::pointlist_type::const_iterator pt = pts.begin (); pt != pts.end (); ++pt) {
    bx += *pt;
  }
  EXPECT_EQ (p.box (), bx);
}

TEST(5) 
{
  db::Path p;

  std::vector <db::Point> c1;
  c1.push_back (db::Point (0, 0));
  c1.push_back (db::Point (0, 1000));
  c1.push_back (db::Point (1000, 2000));
  p.assign (c1.begin (), c1.end ());
  p.extensions (50, 150);
  p.width (200);

  EXPECT_EQ (p.to_string (), "(0,0;0,1000;1000,2000) w=200 bx=50 ex=150 r=false");

  db::Path::pointlist_type pts;
  p.hull (pts);
  EXPECT_EQ (to_string (pts), "(-100,-50;-100,1042;1035,2177;1177,2035;100,958;100,-50)");

  db::Box bx;
  for (db::Path::pointlist_type::const_iterator pt = pts.begin (); pt != pts.end (); ++pt) {
    bx += *pt;
  }
  EXPECT_EQ (p.box (), bx);
}

TEST(6) 
{
  db::Path p;

  std::vector <db::Point> c1;
  c1.push_back (db::Point (0, 0));
  c1.push_back (db::Point (0, 1000));
  c1.push_back (db::Point (1000, 2000));
  p.assign (c1.begin (), c1.end ());
  p.extensions (50, 150);
  p.width (200);
  p.round (true);

  EXPECT_EQ (p.to_string (), "(0,0;0,1000;1000,2000) w=200 bx=50 ex=150 r=true");

  p.width (150);
  EXPECT_EQ (p.to_string (), "(0,0;0,1000;1000,2000) w=150 bx=50 ex=150 r=true");

  p.round(false);
  EXPECT_EQ (p.to_string (), "(0,0;0,1000;1000,2000) w=150 bx=50 ex=150 r=false");

  p.width (200);
  p.round (true);
  EXPECT_EQ (p.to_string (), "(0,0;0,1000;1000,2000) w=200 bx=50 ex=150 r=true");

  db::Path::pointlist_type pts;
  p.hull (pts, 4);
  EXPECT_EQ (to_string (pts), "(-41,-50;-100,-21;-100,1042;973,2115;1077,2135;1135,2077;1115,1973;100,958;100,-21;41,-50)");

  pts.clear ();
  p.hull (pts, 2);
  db::Box bx;
  for (db::Path::pointlist_type::const_iterator pt = pts.begin (); pt != pts.end (); ++pt) {
    bx += *pt;
  }
  EXPECT_EQ (p.box (), bx);
}

TEST(7)
{
  db::DPath path;
  std::string s ("(0,0;0,1000;1000,2000) w=200 bx=50 ex=150 r=true");
  tl::Extractor ex (s.c_str ());
  ex.read (path);

  tl::reuse_vector<db::DPath> v;
  for (int i = 0; i < 10; ++i) {
    v.insert (path);
  }
  EXPECT_EQ (v.begin ()->to_string (), s);
  for (int i = 0; i < 9; ++i) {
    v.erase (v.begin ());
  }
  EXPECT_EQ (v.begin ()->to_string (), s);
  v.clear ();
}

TEST(8a)
{
  db::DPath path;
  std::string s ("(0,0;0,1000;1000,2000) w=200 bx=50 ex=150 r=true");
  tl::Extractor ex (s.c_str ());
  ex.read (path);
  //  434400 = (1000+1000*sqrt(2))*200+200*200*(4.0-M_PI)
  EXPECT_EQ (tl::to_string (path.area ()), "517179.006331");
  EXPECT_EQ (tl::to_string (path.perimeter ()), "5477.2678875");
  EXPECT_EQ (tl::to_string (path.length ()), "2614.21356237");
}

TEST(8b)
{
  db::DPath path;
  std::string s ("(0,0;0,1000;1000,2000) w=200 bx=50 ex=150");
  tl::Extractor ex (s.c_str ());
  ex.read (path);
  EXPECT_EQ (tl::to_string (path.area ()), "522842.712475");
  EXPECT_EQ (tl::to_string (path.perimeter ()), "5628.42712475");
  EXPECT_EQ (tl::to_string (path.length ()), "2614.21356237");
}

TEST(9)
{
  db::Path path;
  {
    std::string s ("(0,0;1100,0;2200,0;3300,0;4400,0;5500,0;6600,0;7700,0;8800,1;9900,1;11000,1;12100,2;13200,2;14300,3;15400,3;16500,4;17600,5;18700,6;19800,7;20900,8;22000,9;23100,11;24200,13;25300,14;26400,16;27500,19;28600,21;29700,23;30800,26;31900,29;33000,32;34100,35;35200,39;36300,43;37400,47;38500,51;39600,55;40700,60;41800,65;42900,70;44000,76;45100,81;46200,88;47300,94;48400,101;49500,108;50600,115;51700,123;52800,131;53900,139;55000,148;56100,157;57200,166;58300,176;59400,186;60500,196;61600,207;62700,218;63800,230;64900,242;66000,254;67100,267;68200,281;69300,294;70400,308;71500,323;72600,338;73700,354;74800,370;75900,386;77000,403;78100,420;79200,438;80300,457;81400,475;82500,495;83600,515;84700,535;85800,556;86900,578;88000,600;89100,622;90200,645;91300,669;92400,693;93500,718;94600,743;95700,769;96800,796;97900,823;99000,851;100100,879;101200,908;102300,938;103400,968;104500,999;105600,1030;106700,1063;107800,1095;108900,1129;110000,1163;111100,1198;112200,1233;113300,1269;114400,1306;115500,1343;116600,1382;117700,1420;118800,1460;119900,1500;121000,1541;122100,1583;123200,1625;124300,1669;125400,1713;126500,1757;127600,1803;128700,1849;129800,1896;130900,1943;132000,1992;133100,2041;134200,2091;135300,2142;136400,2194;137500,2246;138600,2299;139700,2353;140800,2408;141900,2464;143000,2520;144100,2577;145200,2635;146300,2694;147400,2754;148500,2815;149600,2876;150700,2939;151800,3002;152900,3066;154000,3131;155100,3197;156200,3263;157300,3331;158400,3399;159500,3469;160600,3539;161700,3610;162800,3682;163900,3755;165000,3829;166100,3903;167200,3979;168300,4056;169400,4133;170500,4212;171600,4291;172700,4372;173800,4453;174900,4535;176000,4618;177100,4702;178200,4787;179300,4874;180400,4961;181500,5049;182600,5138;183700,5228;184800,5318;185900,5410;187000,5503;188100,5597;189200,5692;190300,5788;191400,5885;192500,5983;193600,6082;194700,6182;195800,6283;196900,6385;198000,6488;199100,6592;200200,6697;201300,6803;202400,6910;203500,7018;204600,7127;205700,7237;206800,7349;207900,7461;209000,7574;210100,7689;211200,7804;212300,7921;213400,8038;214500,8157;215600,8277;216700,8398;217800,8519;218900,8642;220000,8766;221100,8892;222200,9018;223300,9145;224400,9273;225500,9403;226600,9533;227700,9665;228800,9797;229900,9931;231000,10066;232100,10202;233200,10339;234300,10477;235400,10617;236500,10757;237600,10899;238700,11041;239800,11185;240900,11330;242000,11476;243100,11623;244200,11771;245300,11920;246400,12070;247500,12222;248600,12374;249700,12528;250800,12683;251900,12839;253000,12996;254100,13154;255200,13314;256300,13474;257400,13636;258500,13798;259600,13962;260700,14127;261800,14293;262900,14461;264000,14629;265100,14799;266200,14969;267300,15141;268400,15314;269500,15488;270600,15663;271700,15839;272800,16017;273900,16195;275000,16375;276100,16556;277200,16738;278300,16921;279400,17105;280500,17290;281600,17477;282700,17664;283800,17853;284900,18043;286000,18234;287100,18426;288200,18619;289300,18814;290400,19009;291500,19206;292600,19404;293700,19603;294800,19803;295900,20004;297000,20206;298100,20410;299200,20614;300300,20820;301400,21026;302500,21234;303600,21443;304700,21654;305800,21865;306900,22077;308000,22291;309100,22505;310200,22721;311300,22938;312400,23156;313500,23375;314600,23595;315700,23816;316800,24038;317900,24262;319000,24486;320100,24712;321200,24939;322300,25166;323400,25395;324500,25625;325600,25856;326700,26089;327800,26322;328900,26556;330000,26792;331100,27028;332200,27266;333300,27504;334400,27744;335500,27985;336600,28227;337700,28469;338800,28713;339900,28958;341000,29205;342100,29452;343200,29700;344300,29949;345400,30199;346500,30451;347600,30703;348700,30956;349800,31211;350900,31466;352000,31723;353100,31980;354200,32239;355300,32498;356400,32759;357500,33021;358600,33283;359700,33547;360800,33811;361900,34077;363000,34343;364100,34611;365200,34880;366300,35149;367400,35420;368500,35691;369600,35964;370700,36237;371800,36512;372900,36787;374000,37063;375100,37341;376200,37619;377300,37898;378400,38178;379500,38459;380600,38741;381700,39024;382800,39308;383900,39593;385000,39879;386100,40166;387200,40453;388300,40742;389400,41031;390500,41321;391600,41612;392700,41905;393800,42197;394900,42491;396000,42786;397100,43082;398200,43378;399300,43675;400400,43974;401500,44273;402600,44572;403700,44873;404800,45175;405900,45477;407000,45780;408100,46084;409200,46389;410300,46695;411400,47001;412500,47309;413600,47617;414700,47926;415800,48235;416900,48546;418000,48857;419100,49169;420200,49482;421300,49796;422400,50110;423500,50425;424600,50741;425700,51057;426800,51375;427900,51693;429000,52012;430100,52331;431200,52651;432300,52972;433400,53294;434500,53616;435600,53939;436700,54263;437800,54587;438900,54912;440000,55238;441100,55565;442200,55892;443300,56219;444400,56548;445500,56877;446600,57206;447700,57537;448800,57868;449900,58199;451000,58531;452100,58864;453200,59197;454300,59531;455400,59866;456500,60201;457600,60537;458700,60873;459800,61210;460900,61547;462000,61885;463100,62223;464200,62562;465300,62902;466400,63242;467500,63583;468600,63924;469700,64265;470800,64608;471900,64950;473000,65293;474100,65637;475200,65981;476300,66325;477400,66670;478500,67016;479600,67362;480700,67708;481800,68055;482900,68402;484000,68750;485100,69098;486200,69446;487300,69795;488400,70144;489500,70494;490600,70844;491700,71194;492800,71545;493900,71896;495000,72248;496100,72600;497200,72952;498300,73305;499400,73657;500500,74011;501600,74364;502700,74718;503800,75072;504900,75426;506000,75781;507100,76136;508200,76491;509300,76847;510400,77203;511500,77559;512600,77915;513700,78271;514800,78628;515900,78985;517000,79342;518100,79700;519200,80057;520300,80415;521400,80773;522500,81131;523600,81490;524700,81848;525800,82207;526900,82566;528000,82925;529100,83284;530200,83643;531300,84003;532400,84362;533500,84722;534600,85082;535700,85442;536800,85802;537900,86162;539000,86522;540100,86882;541200,87242;542300,87602;543400,87963;544500,88323;545600,88684;546700,89044;547800,89405;548900,89765;550000,90126;551100,90486;552200,90846;553300,91207;554400,91567;555500,91928;556600,92288;557700,92649;558800,93009;559900,93369;561000,93729;562100,94089;563200,94449;564300,94809;565400,95169;566500,95529;567600,95889;568700,96248;569800,96608;570900,96967;572000,97326;573100,97685;574200,98044;575300,98403;576400,98761;577500,99120;578600,99478;579700,99836;580800,100194;581900,100551;583000,100909;584100,101266;585200,101623;586300,101980;587400,102336;588500,102692;589600,103048;590700,103404;591800,103760;592900,104115;594000,104470;595100,104825;596200,105179;597300,105533;598400,105887;599500,106240;600600,106594;601700,106946;602800,107299;603900,107651;605000,108003;606100,108355;607200,108706;608300,109057;609400,109407;610500,109757;611600,110107;612700,110456;613800,110805;614900,111153;616000,111501;617100,111849;618200,112196;619300,112543;620400,112889;621500,113235;622600,113581;623700,113926;624800,114270;625900,114614;627000,114958;628100,115301;629200,115643;630300,115986;631400,116327;632500,116668;633600,117009;634700,117349;635800,117689;636900,118028;638000,118366;639100,118704;640200,119041;641300,119378;642400,119714;643500,120050;644600,120385;645700,120720;646800,121054;647900,121387;649000,121720;650100,122052;651200,122383;652300,122714;653400,123045;654500,123374;655600,123703;656700,124032;657800,124359;658900,124686;660000,125013;661100,125339;662200,125664;663300,125988;664400,126312;665500,126635;666600,126957;667700,127279;668800,127600;669900,127920;671000,128239;672100,128558;673200,128876;674300,129194;675400,129510;676500,129826;677600,130141;678700,130455;679800,130769;680900,131082;682000,131394;683100,131705;684200,132016;685300,132325;686400,132634;687500,132942;688600,133250;689700,133556;690800,133862;691900,134167;693000,134471;694100,134774;695200,135076;696300,135378;697400,135679;698500,135978;699600,136277;700700,136576;701800,136873;702900,137169;704000,137465;705100,137760;706200,138054;707300,138346;708400,138639;709500,138930;710600,139220;711700,139509;712800,139798;713900,140085;715000,140372;716100,140658;717200,140943;718300,141227;719400,141510;720500,141792;721600,142073;722700,142353;723800,142632;724900,142910;726000,143188;727100,143464;728200,143739;729300,144014;730400,144287;731500,144560;732600,144831;733700,145102;734800,145371;735900,145640;737000,145908;738100,146174;739200,146440;740300,146704;741400,146968;742500,147230;743600,147492;744700,147753;745800,148012;746900,148271;748000,148528;749100,148785;750200,149040;751300,149295;752400,149548;753500,149800;754600,150052;755700,150302;756800,150551;757900,150799;759000,151046;760100,151293;761200,151538;762300,151782;763400,152024;764500,152266;765600,152507;766700,152747;767800,152985;768900,153223;770000,153459;771100,153695;772200,153929;773300,154162;774400,154395;775500,154626;776600,154856;777700,155085;778800,155312;779900,155539;781000,155765;782100,155989;783200,156213;784300,156435;785400,156656;786500,156876;787600,157095;788700,157313;789800,157530;790900,157746;792000,157960;793100,158174;794200,158386;795300,158597;796400,158808;797500,159017;798600,159225;799700,159431;800800,159637;801900,159841;803000,160045;804100,160247;805200,160448;806300,160648;807400,160847;808500,161045;809600,161242;810700,161437;811800,161632;812900,161825;814000,162017;815100,162208;816200,162398;817300,162587;818400,162774;819500,162961;820600,163146;821700,163330;822800,163513;823900,163695;825000,163876;826100,164056;827200,164234;828300,164412;829400,164588;830500,164763;831600,164937;832700,165110;833800,165282;834900,165452;836000,165622;837100,165790;838200,165958;839300,166124;840400,166289;841500,166453;842600,166615;843700,166777;844800,166937;845900,167097;847000,167255;848100,167412;849200,167568;850300,167723;851400,167877;852500,168029;853600,168181;854700,168331;855800,168480;856900,168628;858000,168775;859100,168921;860200,169066;861300,169210;862400,169352;863500,169494;864600,169634;865700,169774;866800,169912;867900,170049;869000,170185;870100,170320;871200,170454;872300,170586;873400,170718;874500,170848;875600,170978;876700,171106;877800,171233;878900,171359;880000,171485;881100,171609;882200,171732;883300,171853;884400,171974;885500,172094;886600,172213;887700,172330;888800,172447;889900,172562;891000,172677;892100,172790;893200,172902;894300,173014;895400,173124;896500,173233;897600,173341;898700,173448;899800,173554;900900,173659;902000,173763;903100,173866;904200,173968;905300,174069;906400,174169;907500,174268;908600,174366;909700,174463;910800,174559;911900,174654;913000,174748;914100,174841;915200,174933;916300,175023;917400,175113;918500,175202;919600,175290;920700,175377;921800,175464;922900,175549;924000,175633;925100,175716;926200,175798;927300,175879;928400,175960;929500,176039;930600,176118;931700,176195;932800,176272;933900,176348;935000,176422;936100,176496;937200,176569;938300,176641;939400,176712;940500,176782;941600,176852;942700,176920;943800,176988;944900,177054;946000,177120;947100,177185;948200,177249;949300,177312;950400,177375;951500,177436;952600,177497;953700,177557;954800,177616;955900,177674;957000,177731;958100,177787;959200,177843;960300,177898;961400,177952;962500,178005;963600,178057;964700,178109;965800,178160;966900,178210;968000,178259;969100,178308;970200,178355;971300,178402;972400,178448;973500,178494;974600,178538;975700,178582;976800,178626;977900,178668;979000,178710;980100,178751;981200,178791;982300,178831;983400,178869;984500,178908;985600,178945;986700,178982;987800,179018;988900,179053;990000,179088;991100,179122;992200,179156;993300,179188;994400,179221;995500,179252;996600,179283;997700,179313;998800,179343;999900,179372;1001000,179400;1002100,179428;1003200,179455;1004300,179482;1005400,179508;1006500,179533;1007600,179558;1008700,179582;1009800,179606;1010900,179629;1012000,179651;1013100,179673;1014200,179695;1015300,179716;1016400,179736;1017500,179756;1018600,179776;1019700,179794;1020800,179813;1021900,179831;1023000,179848;1024100,179865;1025200,179881;1026300,179897;1027400,179913;1028500,179928;1029600,179943;1030700,179957;1031800,179970;1032900,179984;1034000,179997;1035100,180009;1036200,180021;1037300,180033;1038400,180044;1039500,180055;1040600,180065;1041700,180075;1042800,180085;1043900,180094;1045000,180103;1046100,180112;1047200,180120;1048300,180128;1049400,180136;1050500,180143;1051600,180150;1052700,180157;1053800,180163;1054900,180170;1056000,180175;1057100,180181;1058200,180186;1059300,180191;1060400,180196;1061500,180200;1062600,180204;1063700,180208;1064800,180212;1065900,180216;1067000,180219;1068100,180222;1069200,180225;1070300,180228;1071400,180230;1072500,180232;1073600,180235;1074700,180237;1075800,180238;1076900,180240;1078000,180242;1079100,180243;1080200,180244;1081300,180245;1082400,180246;1083500,180247;1084600,180248;1085700,180248;1086800,180249;1087900,180249;1089000,180250;1090100,180250;1091200,180250;1092300,180251;1093400,180251;1094500,180251;1095600,180251;1096700,180251;1097800,180251;1098900,180251;1100000,180251) w=15000 bx=0 ex=0 r=false");
    tl::Extractor ex (s.c_str ());
    ex.read (path);
  }

  db::CplxTrans trans;
  {
    std::string s ("r0 *0.0436844933 -27409.4906073,-4721.49865694");
    tl::Extractor ex (s.c_str ());
    ex.read (trans);
  }

  std::string sref = "-27409,-4394,-27073,-4394,-27025,-4394,-26929,-4394,-26881,-4394,-26833,-4394,-26785,-4394,-26737,-4394,-26449,-4393,-26353,-4393,-26305,-4393,-26257,-4393,-26209,-4393,-26113,-4393,-25921,-4392,-25681,-4391,-25537,-4391,-25489,-4391,-25441,-4390,-25393,-4390,-25345,-4390,-25201,-4389,-25057,-4388,-24914,-4387,-24770,-4385,-24674,-4384,-24530,-4383,-24482,-4382,-24434,-4382,-24386,-4381,-24338,-4380,-24243,-4379,-24099,-4377,-24003,-4376,-23955,-4375,-23907,-4374,-23859,-4373,-23716,-4371,-23668,-4370,-23524,-4367,-23476,-4366,-23380,-4364,-23285,-4361,-23237,-4360,-23141,-4358,-23045,-4356,-22997,-4354,-22902,-4352,-22806,-4349,-22758,-4348,-22710,-4346,-22614,-4343,-22519,-4340,-22471,-4339,-22375,-4335,-22327,-4334,-22279,-4332,-22184,-4329,-22136,-4327,-22040,-4323,-21897,-4317,-21801,-4313,-21705,-4309,-21610,-4305,-21562,-4303,-21514,-4301,-21418,-4296,-21371,-4294,-21323,-4291,-21275,-4289,-21179,-4284,-21132,-4282,-21084,-4279,-21036,-4277,-20988,-4274,-20893,-4269,-20797,-4263,-20749,-4260,-20702,-4258,-20606,-4252,-20510,-4246,-20415,-4240,-20367,-4237,-20319,-4234,-20271,-4231,-20176,-4224,-20128,-4221,-20033,-4214,-19937,-4207,-19841,-4200,-19794,-4197,-19746,-4193,-19698,-4189,-19650,-4186,-19555,-4178,-19507,-4174,-19459,-4170,-19364,-4163,-19316,-4159,-19268,-4155,-19220,-4151,-19173,-4146,-19125,-4142,-19077,-4138,-19029,-4134,-18982,-4130,-18934,-4125,-18886,-4121,-18838,-4116,-18791,-4112,-18743,-4107,-18695,-4103,-18647,-4098,-18600,-4094,-18552,-4089,-18504,-4084,-18456,-4079,-18361,-4070,-18313,-4065,-18218,-4055,-18122,-4045,-18075,-4039,-18027,-4034,-17931,-4024,-17884,-4018,-17836,-4013,-17740,-4002,-17692,-3997,-17645,-3991,-17549,-3980,-17454,-3968,-17406,-3962,-17358,-3957,-17311,-3951,-17263,-3945,-17215,-3939,-17120,-3927,-17024,-3914,-16977,-3908,-16929,-3902,-16881,-3895,-16833,-3889,-16786,-3883,-16738,-3876,-16690,-3870,-16595,-3856,-16547,-3850,-16499,-3843,-16452,-3836,-16404,-3829,-16356,-3823,-16261,-3809,-16165,-3795,-16117,-3788,-16070,-3780,-16022,-3773,-15927,-3759,-15831,-3744,-15783,-3736,-15736,-3729,-15688,-3721,-15640,-3714,-15592,-3706,-15497,-3691,-15449,-3683,-15402,-3675,-15354,-3667,-15306,-3659,-15258,-3651,-15211,-3643,-15115,-3627,-15067,-3619,-15020,-3610,-14972,-3602,-14924,-3594,-14877,-3585,-14781,-3569,-14733,-3560,-14686,-3551,-14638,-3543,-14590,-3534,-14542,-3525,-14495,-3517,-14399,-3499,-14304,-3481,-14256,-3472,-14208,-3463,-14113,-3445,-14065,-3435,-13969,-3417,-13922,-3407,-13874,-3398,-13826,-3389,-13779,-3379,-13731,-3370,-13683,-3360,-13635,-3350,-13540,-3331,-13492,-3321,-13396,-3301,-13349,-3291,-13301,-3281,-13253,-3271,-13158,-3251,-13110,-3241,-13015,-3221,-12919,-3200,-12871,-3189,-12824,-3179,-12728,-3158,-12680,-3147,-12633,-3137,-12537,-3115,-12489,-3104,-12441,-3094,-12394,-3083,-12298,-3061,-12250,-3050,-12155,-3028,-12059,-3005,-11964,-2983,-11916,-2972,-11820,-2949,-11725,-2926,-11629,-2903,-11582,-2891,-11486,-2868,-11390,-2844,-11295,-2821,-11199,-2797,-11151,-2785,-11056,-2761,-11008,-2748,-10960,-2736,-10912,-2724,-10865,-2712,-10817,-2699,-10769,-2687,-10721,-2675,-10674,-2662,-10578,-2637,-10482,-2612,-10434,-2600,-10387,-2587,-10339,-2574,-10291,-2562,-10243,-2549,-10195,-2536,-10100,-2510,-10052,-2497,-9908,-2458,-9861,-2445,-9765,-2419,-9717,-2406,-9669,-2393,-9621,-2379,-9526,-2353,-9430,-2326,-9334,-2299,-9239,-2272,-9191,-2258,-9143,-2245,-9047,-2217,-8999,-2204,-8904,-2176,-8808,-2149,-8712,-2121,-8664,-2107,-8617,-2093,-8521,-2065,-8473,-2051,-8377,-2023,-8329,-2009,-8282,-1994,-8138,-1952,-7994,-1909,-7850,-1865,-7803,-1851,-7707,-1822,-7659,-1807,-7563,-1778,-7467,-1749,-7371,-1720,-7276,-1690,-7228,-1675,-7132,-1646,-6988,-1601,-6940,-1586,-6892,-1571,-6844,-1556,-6700,-1511,-6653,-1496,-6509,-1451,-6413,-1421,-6269,-1375,-6173,-1345,-6029,-1299,-5933,-1269,-5790,-1223,-5741,-1207,-5694,-1192,-5646,-1176,-5598,-1161,-5454,-1115,-5310,-1068,-5070,-991,-4926,-944,-4878,-928,-4830,-913,-4686,-866,-4638,-850,-4590,-835,-4350,-756,-4302,-740,-4254,-725,-3821,-583,-3773,-568,-3725,-552,-3677,-536,-3629,-520,-3581,-505,-3533,-489,-3485,-473,-3389,-442,-3341,-426,-3293,-410,-3245,-394,-3197,-379,-3149,-363,-2716,-221,-2668,-206,-2620,-190,-2379,-111,-2331,-96,-2283,-80,-2139,-33,-2091,-17,-2043,-2,-1898,45,-1658,123,-1513,170,-1369,216,-1321,231,-1273,247,-1225,262,-1176,278,-1032,324,-936,355,-791,401,-695,431,-550,477,-454,507,-310,553,-261,568,-117,613,-69,628,-21,643,28,658,172,703,268,733,317,748,413,777,510,807,606,836,702,866,751,880,847,909,895,924,1040,967,1184,1011,1329,1054,1377,1068,1426,1082,1522,1111,1570,1125,1667,1153,1715,1167,1763,1181,1860,1209,1956,1237,2053,1265,2101,1279,2197,1306,2246,1320,2294,1334,2391,1361,2487,1388,2584,1415,2680,1442,2728,1455,2777,1469,2825,1482,2921,1508,2970,1522,3115,1561,3163,1574,3259,1600,3308,1613,3356,1626,3404,1639,3453,1652,3501,1664,3549,1677,3646,1702,3742,1728,3791,1740,3839,1753,3887,1765,3936,1778,3984,1790,4032,1802,4081,1815,4129,1827,4225,1851,4274,1863,4370,1888,4467,1912,4564,1935,4660,1959,4709,1971,4805,1994,4902,2017,4999,2040,5047,2052,5144,2075,5240,2097,5337,2120,5385,2131,5482,2153,5530,2164,5579,2175,5627,2186,5724,2207,5772,2218,5821,2229,5917,2250,5965,2261,6014,2271,6111,2292,6207,2313,6256,2323,6352,2344,6401,2354,6449,2364,6497,2374,6594,2394,6642,2404,6739,2424,6788,2433,6836,2443,6884,2453,6933,2462,6981,2472,7029,2482,7078,2491,7174,2510,7223,2519,7319,2538,7368,2547,7416,2556,7513,2574,7610,2592,7658,2601,7707,2610,7755,2619,7803,2627,7852,2636,7900,2645,7997,2662,8045,2670,8093,2679,8142,2687,8190,2696,8239,2704,8335,2720,8384,2729,8432,2737,8480,2745,8529,2753,8577,2761,8626,2769,8722,2784,8771,2792,8819,2800,8867,2807,8916,2815,8964,2823,9061,2837,9158,2852,9206,2860,9255,2867,9303,2874,9400,2888,9496,2902,9545,2909,9593,2916,9642,2923,9690,2930,9738,2937,9835,2950,9884,2957,9932,2963,9980,2970,10029,2976,10077,2983,10125,2989,10174,2995,10270,3008,10367,3020,10416,3026,10464,3032,10512,3038,10561,3044,10609,3050,10706,3062,10802,3073,10851,3079,10899,3084,10996,3095,11044,3101,11093,3106,11189,3117,11238,3122,11286,3127,11383,3138,11480,3148,11528,3153,11625,3163,11673,3167,11721,3172,11770,3177,11818,3182,11867,3186,11915,3191,11963,3195,12012,3200,12060,3204,12108,3209,12157,3213,12205,3218,12253,3222,12302,3226,12350,3230,12398,3235,12447,3239,12495,3243,12544,3247,12640,3255,12688,3259,12737,3263,12833,3270,12882,3274,12930,3278,12979,3281,13027,3285,13123,3292,13220,3299,13317,3306,13365,3309,13462,3316,13510,3319,13558,3322,13607,3325,13703,3331,13800,3337,13897,3343,13945,3346,13993,3349,14090,3354,14186,3360,14235,3362,14283,3365,14331,3367,14380,3370,14476,3375,14525,3377,14573,3379,14621,3382,14718,3386,14766,3389,14814,3391,14911,3395,15007,3399,15104,3403,15249,3409,15345,3413,15394,3415,15490,3418,15538,3420,15587,3421,15683,3425,15731,3426,15828,3429,15924,3432,15973,3434,16021,3435,16117,3438,16214,3441,16262,3442,16359,3444,16455,3447,16503,3448,16600,3450,16696,3452,16744,3453,16889,3456,16937,3457,17082,3460,17130,3460,17178,3461,17226,3462,17323,3463,17467,3466,17564,3467,17612,3467,17660,3468,17708,3469,17757,3469,17901,3471,17997,3472,18142,3473,18286,3474,18431,3475,18575,3476,18623,3476,18671,3477,18720,3477,18768,3477,18912,3478,19153,3479,19345,3479,19441,3479,19489,3480,19538,3480,19586,3480,19682,3480,19971,3480,20019,3480,20067,3480,20115,3480,20163,3480,20259,3480,20307,3480,20643,3480,20643,2825,20307,2825,20259,2825,20163,2825,20115,2825,20067,2825,20019,2825,19971,2825,19683,2825,19587,2824,19539,2824,19491,2824,19443,2824,19347,2824,19155,2824,18915,2823,18771,2822,18723,2822,18675,2822,18627,2821,18579,2821,18435,2820,18291,2819,18148,2818,18004,2816,17908,2816,17764,2814,17716,2813,17668,2813,17620,2812,17572,2812,17477,2810,17333,2808,17237,2807,17189,2806,17141,2805,17093,2804,16949,2802,16902,2801,16758,2798,16710,2797,16614,2795,16518,2793,16471,2792,16375,2789,16279,2787,16231,2785,16136,2783,16040,2780,15992,2779,15944,2777,15848,2774,15753,2771,15705,2770,15609,2767,15561,2765,15513,2763,15418,2760,15370,2758,15274,2754,15131,2749,15035,2745,14939,2740,14844,2736,14796,2734,14748,2732,14652,2727,14605,2725,14557,2723,14509,2720,14413,2715,14366,2713,14318,2710,14270,2708,14222,2705,14127,2700,14031,2694,13983,2692,13936,2689,13840,2683,13744,2677,13649,2671,13601,2668,13553,2665,13505,2662,13410,2655,13362,2652,13266,2645,13171,2638,13075,2631,13028,2628,12980,2624,12932,2621,12884,2617,12789,2609,12741,2606,12693,2602,12598,2594,12550,2590,12502,2586,12454,2582,12407,2578,12359,2573,12311,2569,12263,2565,12216,2561,12168,2556,12120,2552,12072,2548,12025,2543,11977,2539,11929,2534,11881,2529,11834,2525,11786,2520,11738,2515,11690,2511,11595,2501,11547,2496,11452,2486,11356,2476,11309,2471,11261,2465,11165,2455,11117,2450,11070,2444,10974,2433,10926,2428,10879,2422,10783,2411,10688,2399,10640,2394,10592,2388,10545,2382,10497,2376,10449,2370,10354,2358,10258,2345,10211,2339,10163,2333,10115,2327,10067,2320,10020,2314,9972,2307,9924,2301,9829,2288,9781,2281,9733,2274,9685,2267,9638,2261,9590,2254,9495,2240,9399,2226,9351,2219,9304,2212,9256,2204,9160,2190,9065,2175,9017,2168,8970,2160,8922,2153,8874,2145,8826,2137,8731,2122,8683,2114,8636,2106,8588,2098,8540,2090,8492,2082,8445,2074,8349,2058,8301,2050,8254,2042,8206,2033,8158,2025,8110,2017,8015,2000,7967,1991,7920,1983,7872,1974,7824,1965,7776,1957,7729,1948,7633,1930,7538,1912,7490,1903,7442,1894,7347,1876,7299,1867,7203,1848,7156,1839,7108,1829,7060,1820,7012,1810,6965,1801,6917,1791,6869,1781,6774,1762,6726,1752,6630,1732,6583,1723,6535,1713,6487,1703,6392,1682,6344,1672,6249,1652,6153,1631,6105,1621,6058,1610,5962,1589,5914,1579,5867,1568,5771,1546,5723,1536,5675,1525,5628,1514,5532,1492,5484,1481,5389,1459,5293,1437,5198,1414,5150,1403,5054,1380,4959,1357,4863,1334,4816,1322,4720,1299,4624,1275,4529,1252,4433,1228,4385,1216,4290,1192,4242,1180,4194,1167,4146,1155,4099,1143,4051,1131,4003,1118,3955,1106,3908,1094,3812,1069,3716,1043,3668,1031,3621,1018,3573,1005,3525,993,3477,980,3429,967,3334,941,3286,929,3142,889,3095,876,2999,850,2951,837,2903,824,2855,811,2760,784,2664,757,2568,730,2473,703,2425,690,2377,676,2281,649,2233,635,2138,607,2042,580,1946,552,1898,538,1851,524,1755,496,1707,482,1611,454,1563,440,1516,426,1372,383,1228,340,1084,297,1037,282,941,253,893,239,797,209,701,180,605,151,510,121,462,107,366,77,222,32,174,17,126,3,78,-12,-66,-57,-113,-72,-257,-118,-353,-148,-497,-193,-593,-224,-737,-270,-833,-300,-976,-346,-1025,-362,-1072,-377,-1120,-392,-1168,-408,-1312,-454,-1456,-501,-1696,-578,-1840,-625,-1888,-641,-1936,-656,-2080,-703,-2128,-719,-2176,-734,-2416,-813,-2464,-828,-2512,-844,-2945,-986,-2993,-1001,-3041,-1017,-3089,-1033,-3137,-1049,-3185,-1064,-3281,-1096,-3329,-1112,-3377,-1127,-3425,-1143,-3473,-1159,-3521,-1174,-3569,-1190,-3617,-1206,-4050,-1348,-4098,-1363,-4146,-1379,-4387,-1458,-4435,-1473,-4483,-1489,-4627,-1536,-4675,-1551,-4723,-1567,-4868,-1614,-5108,-1692,-5253,-1738,-5397,-1785,-5445,-1800,-5493,-1816,-5542,-1831,-5590,-1847,-5734,-1893,-5830,-1924,-5975,-1970,-6071,-2000,-6216,-2046,-6312,-2076,-6456,-2122,-6505,-2137,-6649,-2182,-6697,-2197,-6745,-2212,-6794,-2227,-6938,-2272,-7035,-2302,-7083,-2316,-7179,-2346,-7276,-2376,-7372,-2405,-7468,-2434,-7517,-2449,-7613,-2478,-7661,-2493,-7806,-2536,-7951,-2580,-8095,-2623,-8143,-2637,-8192,-2651,-8288,-2680,-8336,-2694,-8433,-2722,-8481,-2736,-8529,-2750,-8626,-2778,-8722,-2806,-8819,-2834,-8867,-2847,-8963,-2875,-9012,-2889,-9060,-2902,-9157,-2930,-9253,-2957,-9350,-2984,-9446,-3011,-9494,-3024,-9543,-3037,-9591,-3051,-9687,-3077,-9736,-3090,-9881,-3130,-9929,-3143,-10025,-3169,-10074,-3182,-10122,-3195,-10170,-3208,-10219,-3220,-10267,-3233,-10315,-3246,-10412,-3271,-10508,-3296,-10557,-3309,-10605,-3322,-10653,-3334,-10702,-3346,-10750,-3359,-10798,-3371,-10847,-3383,-10895,-3396,-10991,-3420,-11040,-3432,-11136,-3456,-11233,-3480,-11330,-3504,-11426,-3528,-11475,-3540,-11571,-3563,-11668,-3586,-11765,-3609,-11813,-3621,-11910,-3643,-12006,-3666,-12103,-3688,-12151,-3700,-12248,-3722,-12296,-3733,-12345,-3744,-12393,-3755,-12490,-3776,-12538,-3787,-12587,-3798,-12683,-3819,-12731,-3830,-12780,-3840,-12877,-3861,-12973,-3882,-13022,-3892,-13118,-3913,-13167,-3923,-13215,-3933,-13263,-3943,-13360,-3963,-13408,-3973,-13505,-3993,-13554,-4002,-13602,-4012,-13650,-4022,-13699,-4031,-13747,-4041,-13795,-4050,-13844,-4060,-13940,-4079,-13989,-4088,-14085,-4107,-14134,-4116,-14182,-4125,-14279,-4143,-14376,-4161,-14424,-4170,-14473,-4179,-14521,-4188,-14569,-4196,-14618,-4205,-14666,-4214,-14763,-4231,-14811,-4239,-14860,-4248,-14908,-4256,-14956,-4264,-15005,-4273,-15101,-4289,-15150,-4297,-15198,-4305,-15247,-4314,-15295,-4322,-15343,-4329,-15392,-4337,-15488,-4353,-15537,-4361,-15585,-4369,-15634,-4376,-15682,-4384,-15730,-4391,-15827,-4406,-15924,-4421,-15972,-4428,-16021,-4436,-16069,-4443,-16166,-4457,-16262,-4471,-16311,-4478,-16359,-4485,-16408,-4492,-16456,-4499,-16504,-4505,-16601,-4519,-16650,-4525,-16698,-4532,-16746,-4538,-16795,-4545,-16843,-4551,-16891,-4558,-16940,-4564,-17036,-4577,-17133,-4589,-17182,-4595,-17230,-4601,-17278,-4607,-17327,-4613,-17375,-4619,-17472,-4630,-17569,-4642,-17617,-4647,-17665,-4653,-17762,-4664,-17810,-4670,-17859,-4675,-17955,-4686,-18004,-4691,-18052,-4696,-18149,-4706,-18246,-4717,-18294,-4722,-18391,-4731,-18439,-4736,-18488,-4741,-18536,-4746,-18584,-4750,-18633,-4755,-18681,-4760,-18729,-4764,-18778,-4769,-18826,-4773,-18874,-4778,-18923,-4782,-18971,-4786,-19019,-4791,-19068,-4795,-19116,-4799,-19164,-4803,-19213,-4808,-19261,-4812,-19310,-4816,-19406,-4824,-19454,-4828,-19503,-4831,-19599,-4839,-19648,-4843,-19696,-4846,-19745,-4850,-19793,-4854,-19889,-4861,-19986,-4868,-20083,-4875,-20131,-4878,-20228,-4884,-20276,-4888,-20324,-4891,-20373,-4894,-20469,-4900,-20566,-4906,-20663,-4912,-20711,-4915,-20759,-4917,-20856,-4923,-20952,-4928,-21001,-4931,-21049,-4934,-21097,-4936,-21146,-4939,-21242,-4944,-21291,-4946,-21339,-4948,-21387,-4951,-21484,-4955,-21532,-4957,-21580,-4960,-21677,-4964,-21773,-4968,-21870,-4972,-22015,-4978,-22111,-4982,-22160,-4983,-22256,-4987,-22304,-4989,-22353,-4990,-22449,-4994,-22498,-4995,-22594,-4998,-22690,-5001,-22739,-5003,-22787,-5004,-22883,-5007,-22980,-5009,-23028,-5011,-23125,-5013,-23221,-5015,-23269,-5017,-23366,-5019,-23462,-5021,-23511,-5022,-23655,-5025,-23703,-5026,-23848,-5028,-23896,-5029,-23944,-5030,-23993,-5031,-24089,-5032,-24233,-5034,-24330,-5036,-24378,-5036,-24426,-5037,-24474,-5037,-24523,-5038,-24667,-5040,-24763,-5041,-24908,-5042,-25052,-5043,-25197,-5044,-25341,-5045,-25389,-5045,-25438,-5046,-25486,-5046,-25534,-5046,-25678,-5047,-25919,-5048,-26111,-5048,-26207,-5048,-26255,-5048,-26304,-5049,-26352,-5049,-26448,-5049,-26737,-5049,-26785,-5049,-26833,-5049,-26881,-5049,-26929,-5049,-27025,-5049,-27073,-5049,-27409,-5049";

  db::DPath::pointlist_type pts;
  path.transformed (trans).hull (pts);
  std::string sres;
  for (size_t i = 0; i < pts.size(); ++i) {
    if (i > 0) {
      sres += ",";
    }
    sres += db::Point(pts[i]).to_string();
  }

  EXPECT_EQ (sref, sres);
}

TEST(10)
{
  db::Path path;
  std::string s ("(0,0;0,1000000000;1000000000,2000000000) w=200000000 bx=50000000 ex=150000000 r=false");
  tl::Extractor ex (s.c_str ());
  ex.read (path);
  //  434400 = (1000+1000*sqrt(2))*200+200*200*(4.0-M_PI)
  EXPECT_EQ (tl::to_string (path.area ()), "522842712400000000");
  EXPECT_EQ (tl::to_string (path.perimeter ()), "5628427125");
}

TEST(11)
{
  db::Path path;
  db::Path::pointlist_type pts;

  tl::Extractor ("(0,0;1000,0;0,0) w=0 bx=0 ex=0 r=false").read (path);
  EXPECT_EQ (tl::to_string (path.box ()), "(0,0;1000,0)");
  EXPECT_EQ (tl::to_string (path.perimeter ()), "4000");
  EXPECT_EQ (tl::to_string (path.area ()), "0");

  pts.clear();
  path.hull (pts, 4);
  EXPECT_EQ (to_string (pts), "(0,0;1000,0;1000,0;0,0;0,0;1000,0;1000,0;0,0)");

  tl::Extractor ("(0,0;1000,0;0,0) w=0 bx=100 ex=0 r=false").read (path);
  EXPECT_EQ (tl::to_string (path.box ()), "(-100,0;1000,0)");
  EXPECT_EQ (tl::to_string (path.perimeter ()), "4200");
  EXPECT_EQ (tl::to_string (path.area ()), "0");

  pts.clear();
  path.hull (pts, 4);
  EXPECT_EQ (to_string (pts), "(-100,0;1000,0;1000,0;0,0;0,0;1000,0;1000,0;-100,0)");

  tl::Extractor ("(0,0;1000,0;0,0) w=20 bx=100 ex=0 r=false").read (path);
  EXPECT_EQ (tl::to_string (path.box ()), "(-100,-10;1010,10)");
  EXPECT_EQ (tl::to_string (path.perimeter ()), "4240");
  EXPECT_EQ (tl::to_string (path.area ()), "42000");

  pts.clear();
  path.hull (pts, 4);
  EXPECT_EQ (to_string (pts), "(-100,10;1010,10;1010,-10;0,-10;0,10;1010,10;1010,-10;-100,-10)");
}

