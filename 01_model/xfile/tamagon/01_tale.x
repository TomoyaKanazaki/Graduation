xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 122;
 0.00535;0.01858;-2.52554;,
 3.28563;9.81369;-0.36466;,
 4.64436;0.01858;-2.52554;,
 0.00535;0.01858;-2.52554;,
 0.00535;11.36720;0.41373;,
 0.00535;0.01858;-2.52554;,
 -3.27493;9.81369;-0.36466;,
 0.00535;0.01858;-2.52554;,
 -4.63365;0.01858;-2.52554;,
 0.00535;0.01858;-2.52554;,
 -3.27493;-6.54098;-6.16163;,
 0.00535;0.01858;-2.52554;,
 0.00535;-8.09450;-6.94002;,
 0.00535;0.01858;-2.52554;,
 3.28563;-6.54098;-6.16163;,
 0.00535;0.01858;-2.52554;,
 4.64436;0.01858;-2.52554;,
 3.28563;0.02600;11.74623;,
 2.42515;-0.50556;17.56157;,
 3.42748;-3.48776;17.24812;,
 4.64436;-3.72454;9.86705;,
 0.00535;1.57951;12.52461;,
 0.00535;0.72970;17.69140;,
 4.64436;-3.72454;9.86705;,
 3.42748;-3.48776;17.24812;,
 2.42516;-6.46994;16.93468;,
 3.28563;-7.47504;7.98785;,
 -3.27492;0.02600;11.74623;,
 -2.41444;-0.50556;17.56156;,
 0.00535;-7.70520;16.80485;,
 0.00535;-9.02855;7.20946;,
 -4.63365;-3.72454;9.86705;,
 -3.41677;-3.48776;17.24812;,
 -2.41444;-6.46994;16.93470;,
 -3.27493;-7.47504;7.98785;,
 0.50574;-0.29774;26.99104;,
 0.71300;-0.90192;27.13053;,
 0.00535;-0.90192;27.13054;,
 0.00535;-0.04747;26.93326;,
 0.00535;-0.90192;27.13054;,
 -0.49503;-0.29774;26.99104;,
 0.00535;-0.90192;27.13054;,
 -0.70229;-0.90192;27.13054;,
 0.00535;-0.90192;27.13054;,
 -0.49502;-1.50610;27.27002;,
 0.00535;-0.90192;27.13054;,
 0.00535;-1.75636;27.32783;,
 0.00535;-0.90192;27.13054;,
 0.50575;-1.50610;27.27004;,
 0.00535;-0.90192;27.13054;,
 0.71300;-0.90192;27.13053;,
 0.00535;-0.90192;27.13054;,
 3.28563;1.92755;5.45059;,
 4.64436;-1.82297;3.57139;,
 0.00535;3.48107;6.22898;,
 4.64436;-1.82297;3.57139;,
 3.28563;-7.54404;1.69222;,
 -3.27493;1.92755;5.45059;,
 0.00535;-9.09756;0.91383;,
 -4.63365;-1.82297;3.57139;,
 -3.27493;-7.54404;1.69222;,
 0.06599;0.87439;8.53607;,
 2.23960;0.87439;8.53607;,
 1.60297;0.22859;11.44904;,
 0.06599;0.87439;8.53607;,
 0.06599;-0.03890;12.65563;,
 0.06599;0.87439;8.53607;,
 -1.47100;0.22859;11.44904;,
 0.06599;0.87439;8.53607;,
 -2.10763;0.87439;8.53607;,
 0.06599;0.87439;8.53607;,
 -1.47100;1.52018;5.62311;,
 0.06599;0.87439;8.53607;,
 0.06599;1.78765;4.41651;,
 0.06599;0.87439;8.53607;,
 1.60297;1.52018;5.62312;,
 0.06599;0.87439;8.53607;,
 2.23960;0.87439;8.53607;,
 0.06598;6.60097;6.53686;,
 0.06598;8.04901;8.70548;,
 0.37291;7.96107;9.10223;,
 0.99936;6.39866;7.44941;,
 -0.86741;6.39866;7.44941;,
 -0.24095;7.96107;9.10223;,
 0.50003;7.74872;10.06007;,
 1.38598;5.91023;9.65252;,
 -1.25402;5.91023;9.65252;,
 -0.36807;7.74872;10.06007;,
 1.38598;5.91023;9.65252;,
 0.50003;7.74872;10.06007;,
 0.37291;7.53637;11.01794;,
 0.99936;5.42184;11.85560;,
 -0.86740;5.42184;11.85560;,
 -0.24095;7.53637;11.01794;,
 0.06598;7.44841;11.41470;,
 0.06598;5.21953;12.76816;,
 0.26936;8.20547;10.16134;,
 0.20979;8.14504;10.43391;,
 0.06598;8.20547;10.16134;,
 0.06598;8.12001;10.54679;,
 0.06598;8.20547;10.16134;,
 -0.07784;8.14504;10.43390;,
 0.06598;8.20547;10.16134;,
 -0.13740;8.20547;10.16134;,
 0.06598;8.20547;10.16134;,
 -0.07784;8.26590;9.88879;,
 0.06598;8.20547;10.16134;,
 0.06598;8.29092;9.77589;,
 0.06598;8.20547;10.16134;,
 0.20980;8.26590;9.88880;,
 0.06598;8.20547;10.16134;,
 0.26936;8.20547;10.16134;,
 0.06598;8.20547;10.16134;,
 1.37541;4.32469;6.31005;,
 1.91778;3.69264;9.16088;,
 0.06598;4.58645;5.12918;,
 1.91778;3.69264;9.16088;,
 1.37541;3.06063;12.01173;,
 -1.24344;4.32469;6.31005;,
 0.06598;2.79885;13.19260;,
 -1.78582;3.69264;9.16089;,
 -1.24345;3.06063;12.01173;;
 
 96;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;17,18,19,20;,
 4;17,21,22,18;,
 4;23,24,25,26;,
 4;21,27,28,22;,
 4;26,25,29,30;,
 4;27,31,32,28;,
 4;30,29,33,34;,
 4;31,34,33,32;,
 4;35,36,19,18;,
 3;37,36,35;,
 4;38,35,18,22;,
 3;39,35,38;,
 4;40,38,22,28;,
 3;41,38,40;,
 4;42,40,28,32;,
 3;43,40,42;,
 4;44,42,32,33;,
 3;45,42,44;,
 4;46,44,33,29;,
 3;47,44,46;,
 4;48,46,29,25;,
 3;49,46,48;,
 4;50,48,25,24;,
 3;51,48,50;,
 4;52,17,20,53;,
 4;52,53,2,1;,
 4;52,1,4,54;,
 4;52,54,21,17;,
 4;55,23,26,56;,
 4;55,56,14,16;,
 4;54,4,6,57;,
 4;54,57,27,21;,
 4;56,26,30,58;,
 4;56,58,12,14;,
 4;57,6,8,59;,
 4;57,59,31,27;,
 4;58,30,34,60;,
 4;58,60,10,12;,
 4;59,8,10,60;,
 4;59,60,34,31;,
 3;61,62,63;,
 3;64,63,65;,
 3;66,65,67;,
 3;68,67,69;,
 3;70,69,71;,
 3;72,71,73;,
 3;74,73,75;,
 3;76,75,77;,
 4;78,79,80,81;,
 4;78,82,83,79;,
 4;81,80,84,85;,
 4;82,86,87,83;,
 4;88,89,90,91;,
 4;86,92,93,87;,
 4;91,90,94,95;,
 4;92,95,94,93;,
 4;96,97,90,89;,
 3;98,97,96;,
 4;97,99,94,90;,
 3;100,99,97;,
 4;99,101,93,94;,
 3;102,101,99;,
 4;101,103,87,93;,
 3;104,103,101;,
 4;103,105,83,87;,
 3;106,105,103;,
 4;105,107,79,83;,
 3;108,107,105;,
 4;107,109,80,79;,
 3;110,109,107;,
 4;109,111,84,80;,
 3;112,111,109;,
 4;113,81,85,114;,
 4;113,114,77,75;,
 4;113,75,73,115;,
 4;113,115,78,81;,
 4;116,88,91,117;,
 4;116,117,63,62;,
 4;115,73,71,118;,
 4;115,118,82,78;,
 4;117,91,95,119;,
 4;117,119,65,63;,
 4;118,71,69,120;,
 4;118,120,86,82;,
 4;119,95,92,121;,
 4;119,121,67,65;,
 4;120,69,67,121;,
 4;120,121,92,86;;
 
 MeshMaterialList {
  7;
  96;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\dw002.jpg";
   }
  }
  Material {
   0.184800;0.580000;0.116000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.680800;0.216800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.702400;0.702400;0.702400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.081600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  120;
  -0.000000;0.360627;-0.932710;,
  0.000001;0.353810;-0.935317;,
  -0.055241;0.232705;-0.970977;,
  -0.000000;0.250725;-0.968058;,
  0.055241;0.232705;-0.970977;,
  -0.000001;0.353810;-0.935317;,
  0.008956;0.481367;-0.876474;,
  -0.000000;0.477948;-0.878388;,
  -0.008957;0.481367;-0.876474;,
  0.977059;-0.010386;0.212717;,
  0.750459;0.643668;0.150012;,
  -0.000001;0.992499;0.122248;,
  -0.750459;0.643667;0.150012;,
  -0.977058;-0.010390;0.212719;,
  -0.719799;-0.632571;0.285909;,
  -0.000004;-0.944246;0.329241;,
  0.719799;-0.632570;0.285909;,
  0.760576;0.625018;0.175720;,
  0.995629;-0.011933;0.092632;,
  -0.000000;0.973960;0.226721;,
  0.745436;-0.663143;0.067578;,
  -0.760576;0.625017;0.175720;,
  0.000000;-0.996837;0.079476;,
  -0.995629;-0.011932;0.092632;,
  -0.745436;-0.663143;0.067578;,
  0.964528;0.020585;0.263178;,
  0.742100;0.656400;0.135744;,
  -0.000001;0.996736;0.080735;,
  -0.742101;0.656399;0.135744;,
  -0.964527;0.020576;0.263181;,
  -0.704358;-0.576317;0.414413;,
  -0.000007;-0.866779;0.498692;,
  0.704359;-0.576316;0.414413;,
  -0.000001;0.224967;0.974366;,
  0.783239;0.525727;0.331886;,
  0.999480;0.009254;0.030880;,
  -0.000000;0.815227;0.579142;,
  0.778938;-0.624862;-0.052941;,
  -0.783239;0.525726;0.331886;,
  -0.000000;-0.998339;-0.057610;,
  -0.999480;0.009254;0.030880;,
  -0.778939;-0.624862;-0.052941;,
  0.000008;0.224967;0.974367;,
  0.000005;0.224962;0.974367;,
  -0.000000;0.224965;0.974367;,
  -0.000001;0.224966;0.974367;,
  0.000000;0.224957;0.974369;,
  -0.000021;0.224964;0.974367;,
  -0.000012;0.224981;0.974363;,
  0.000013;0.224977;0.974364;,
  0.999299;0.001381;0.037414;,
  0.779860;0.412361;0.470931;,
  0.000000;0.593491;0.804841;,
  0.792044;-0.604180;-0.087370;,
  -0.779861;0.412361;0.470931;,
  -0.000000;-0.991943;-0.126686;,
  -0.999299;0.001381;0.037414;,
  -0.792044;-0.604180;-0.087371;,
  0.898561;0.428445;0.094985;,
  0.742717;0.500345;0.445002;,
  0.236900;0.688364;0.685590;,
  -0.742715;0.500348;0.445002;,
  -0.898562;0.428445;0.094984;,
  -0.742711;0.641537;-0.191860;,
  0.535144;0.779807;-0.324842;,
  0.742713;0.641535;-0.191860;,
  0.000000;-0.976296;-0.216442;,
  -0.000002;-0.976296;-0.216442;,
  0.000000;-0.976296;-0.216441;,
  0.000002;-0.976295;-0.216442;,
  -0.000001;-0.976296;-0.216442;,
  -0.000006;-0.976297;-0.216437;,
  0.000001;-0.976297;-0.216434;,
  0.000006;-0.976297;-0.216437;,
  0.000000;-0.976296;-0.216440;,
  0.650568;0.559261;-0.513798;,
  0.826639;0.469891;-0.309629;,
  -0.826639;0.469891;-0.309630;,
  0.946920;0.313849;0.069579;,
  -0.946920;0.313849;0.069579;,
  0.826643;0.295011;0.479198;,
  -0.826641;0.295011;0.479201;,
  0.650578;0.289723;0.702003;,
  -0.000003;0.976293;0.216453;,
  0.887963;0.449012;0.099547;,
  0.490110;0.779636;0.389821;,
  -0.000000;0.902888;0.429875;,
  -0.367969;0.859995;0.353563;,
  -0.887964;0.449010;0.099545;,
  -0.490093;0.871345;-0.023807;,
  -0.000001;0.999968;-0.008025;,
  0.367954;0.928848;0.043024;,
  0.860325;0.273413;-0.430217;,
  0.987203;0.155689;0.034516;,
  0.662669;0.327025;-0.673739;,
  0.860326;0.065976;0.505456;,
  -0.860324;0.273411;-0.430219;,
  0.662674;0.011649;0.748818;,
  -0.987203;0.155687;0.034516;,
  -0.860326;0.065974;0.505457;,
  -0.650566;0.559262;-0.513799;,
  -0.535137;0.779810;-0.324846;,
  -0.650573;0.289723;0.702008;,
  -0.624921;0.429636;0.651834;,
  0.000000;0.976294;0.216451;,
  0.000000;0.976291;0.216461;,
  0.000000;0.976291;0.216460;,
  0.000000;0.976290;0.216465;,
  0.995314;0.094400;0.020928;,
  0.861113;0.180910;-0.475138;,
  0.652341;0.207402;-0.728996;,
  0.861114;-0.036847;0.507076;,
  -0.662668;0.327025;-0.673740;,
  -0.652341;0.207402;-0.728997;,
  -0.861113;0.180909;-0.475139;,
  0.652343;-0.120125;0.748344;,
  -0.995315;0.094397;0.020928;,
  -0.662673;0.011647;0.748819;,
  -0.861114;-0.036851;0.507075;,
  -0.652344;-0.120129;0.748343;;
  96;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;17,10,9,18;,
  4;17,19,11,10;,
  4;18,9,16,20;,
  4;19,21,12,11;,
  4;20,16,15,22;,
  4;21,23,13,12;,
  4;22,15,14,24;,
  4;23,24,14,13;,
  4;26,25,9,10;,
  3;33,42,43;,
  4;27,26,10,11;,
  3;33,43,44;,
  4;28,27,11,12;,
  3;33,44,45;,
  4;29,28,12,13;,
  3;33,45,46;,
  4;30,29,13,14;,
  3;33,46,47;,
  4;31,30,14,15;,
  3;33,47,48;,
  4;32,31,15,16;,
  3;33,48,49;,
  4;25,32,16,9;,
  3;33,49,42;,
  4;34,17,18,35;,
  4;34,35,50,51;,
  4;34,51,52,36;,
  4;34,36,19,17;,
  4;35,18,20,37;,
  4;35,37,53,50;,
  4;36,52,54,38;,
  4;36,38,21,19;,
  4;37,20,22,39;,
  4;37,39,55,53;,
  4;38,54,56,40;,
  4;38,40,23,21;,
  4;39,22,24,41;,
  4;39,41,57,55;,
  4;40,56,57,41;,
  4;40,41,24,23;,
  3;74,66,67;,
  3;74,67,68;,
  3;74,68,69;,
  3;74,69,70;,
  3;74,70,71;,
  3;74,71,72;,
  3;74,72,73;,
  3;74,73,66;,
  4;75,64,65,76;,
  4;100,77,63,101;,
  4;76,65,58,78;,
  4;77,79,62,63;,
  4;78,58,59,80;,
  4;79,81,61,62;,
  4;80,59,60,82;,
  4;81,102,103,61;,
  4;84,85,59,58;,
  3;83,104,105;,
  4;85,86,60,59;,
  3;83,86,85;,
  4;86,87,61,60;,
  3;83,87,86;,
  4;87,88,62,61;,
  3;83,106,87;,
  4;88,89,63,62;,
  3;83,107,106;,
  4;89,90,101,63;,
  3;83,90,89;,
  4;90,91,65,64;,
  3;83,91,90;,
  4;91,84,58,65;,
  3;83,105,91;,
  4;92,76,78,93;,
  4;92,93,108,109;,
  4;92,109,110,94;,
  4;92,94,75,76;,
  4;93,78,80,95;,
  4;93,95,111,108;,
  4;112,113,114,96;,
  4;112,96,77,100;,
  4;95,80,82,97;,
  4;95,97,115,111;,
  4;96,114,116,98;,
  4;96,98,79,77;,
  4;117,102,81,99;,
  4;117,99,118,119;,
  4;98,116,118,99;,
  4;98,99,81,79;;
 }
 MeshTextureCoords {
  122;
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.125000;0.802600;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.802600;,
  0.250000;0.802600;,
  0.250000;1.000000;,
  1.000000;0.802600;,
  1.000000;1.000000;,
  0.875000;1.000000;,
  0.875000;0.802600;,
  0.375000;0.802600;,
  0.375000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.802600;,
  0.500000;0.802600;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.802600;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.062500;1.000000;,
  0.250000;1.000000;,
  0.187500;1.000000;,
  0.375000;1.000000;,
  0.312500;1.000000;,
  0.500000;1.000000;,
  0.437500;1.000000;,
  0.625000;1.000000;,
  0.562500;1.000000;,
  0.750000;1.000000;,
  0.687500;1.000000;,
  0.875000;1.000000;,
  0.812500;1.000000;,
  1.000000;1.000000;,
  0.937500;1.000000;,
  0.125000;0.401300;,
  0.000000;0.401300;,
  0.250000;0.401300;,
  1.000000;0.401300;,
  0.875000;0.401300;,
  0.375000;0.401300;,
  0.750000;0.401300;,
  0.500000;0.401300;,
  0.625000;0.401300;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.750000;0.500000;,
  0.750000;0.000000;,
  0.875000;0.000000;,
  0.875000;0.500000;,
  0.625000;0.500000;,
  0.625000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;0.500000;,
  0.500000;0.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.500000;,
  0.375000;0.500000;,
  0.375000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.500000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.062500;0.000000;,
  0.250000;0.000000;,
  0.187500;0.000000;,
  0.375000;0.000000;,
  0.312500;0.000000;,
  0.500000;0.000000;,
  0.437500;0.000000;,
  0.625000;0.000000;,
  0.562500;0.000000;,
  0.750000;0.000000;,
  0.687500;0.000000;,
  0.875000;0.000000;,
  0.812500;0.000000;,
  1.000000;0.000000;,
  0.937500;0.000000;,
  0.875000;0.811500;,
  1.000000;0.811500;,
  0.750000;0.811500;,
  0.000000;0.811500;,
  0.125000;0.811500;,
  0.625000;0.811500;,
  0.250000;0.811500;,
  0.500000;0.811500;,
  0.375000;0.811500;;
 }
}