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
 186;
 -1.47242;4.73119;0.00000;,
 -1.47242;4.72786;0.85530;,
 -0.90597;4.70021;0.85530;,
 -0.90597;4.70355;0.00000;,
 -1.47242;4.83476;3.19990;,
 -0.90596;4.80713;3.19990;,
 -2.46679;4.88594;0.08748;,
 -2.46070;4.89626;0.85530;,
 -2.28225;5.01937;2.67321;,
 -9.95324;1.11138;-0.28365;,
 -9.98887;1.11138;0.91800;,
 -7.18893;2.25550;1.18655;,
 -7.07697;2.25550;0.09337;,
 -9.51965;1.11138;4.95297;,
 -6.62915;2.25550;3.98281;,
 -6.40724;0.13676;-0.38364;,
 -6.40980;0.13698;0.54945;,
 -8.76347;0.29535;0.55617;,
 -8.73710;0.29525;-0.29295;,
 -6.41873;0.13794;3.93569;,
 -8.43889;0.29576;3.40451;,
 -4.61884;-0.02026;-0.45254;,
 -4.60131;-0.02019;0.54328;,
 -5.03584;-0.01997;4.27613;,
 -6.60542;0.88340;5.73092;,
 -3.80202;2.25550;4.62559;,
 -2.00400;2.25550;4.62559;,
 -4.59939;0.65550;6.23672;,
 -9.81071;1.12829;-4.62206;,
 -6.62915;2.25550;-4.27936;,
 -3.80202;2.25550;-4.92215;,
 -6.60543;0.49776;-5.24620;,
 -2.00400;2.25550;-4.92215;,
 -4.11368;0.29962;-5.71817;,
 0.00000;4.59187;0.85529;,
 0.00000;4.61771;-0.00472;,
 0.00000;4.69903;3.27231;,
 0.00000;0.18685;6.54655;,
 0.00000;2.25550;5.30650;,
 -1.82262;-0.30932;0.59475;,
 -1.81419;-0.31012;4.49503;,
 -1.82514;-0.30908;-0.55001;,
 0.00000;2.17411;-5.03821;,
 0.00000;0.00000;-6.94327;,
 -2.86305;3.06268;-4.53891;,
 -1.06503;3.06268;-4.53891;,
 -1.06503;3.06268;4.20214;,
 -2.86305;3.06268;4.20214;,
 -5.49555;3.06268;1.30139;,
 -5.38360;3.06268;0.24882;,
 -4.93578;3.06268;-3.96143;,
 -4.93577;3.06268;3.62466;,
 0.00000;3.16684;-4.50692;,
 0.00000;3.35625;4.49050;,
 -2.07834;3.85227;-3.87173;,
 -0.60361;3.85227;-3.87173;,
 -0.60360;3.85227;3.75168;,
 -2.07834;3.85227;3.54009;,
 -3.96941;3.85227;1.05214;,
 -3.87758;3.85227;0.16809;,
 -3.51028;3.85227;-3.36809;,
 -3.77838;3.85227;3.24804;,
 0.00000;4.14584;-3.74744;,
 0.00000;4.14584;3.97461;,
 -1.33961;4.76136;-3.42118;,
 -0.90597;4.67706;-3.42118;,
 -2.07357;4.95047;-2.69046;,
 0.00000;4.58849;-3.44477;,
 -4.69071;-0.52354;-3.97849;,
 -1.82677;-0.81217;-4.67141;,
 -1.82892;-0.30870;-2.26714;,
 -4.64513;-0.02035;-1.94627;,
 -6.40596;-0.36645;-3.64938;,
 -6.40340;0.13644;-1.78326;,
 -8.63730;-0.20798;-3.21312;,
 -8.69753;0.29510;-1.56662;,
 -9.89979;1.11138;-2.08612;,
 -6.90904;2.25550;-1.54641;,
 -5.21567;3.06268;-1.33003;,
 -3.73984;3.85227;-1.15798;,
 -2.47594;4.87046;-1.06423;,
 -1.47242;4.73617;-1.28294;,
 -0.90597;4.70854;-1.28294;,
 0.00000;4.65645;-1.29474;,
 -9.26168;0.62256;-0.28855;,
 -9.29211;0.62263;0.70819;,
 -8.90796;0.62293;4.05303;,
 -6.51888;0.43616;4.68492;,
 -4.87995;0.24938;5.09220;,
 -1.09353;-0.09162;5.34918;,
 -1.09963;-0.09100;0.75924;,
 0.00000;-0.62248;0.99063;,
 -1.10145;-0.09083;-0.58845;,
 0.00000;-0.62248;-0.64165;,
 0.00000;-0.62248;-3.09007;,
 -1.10418;-0.09059;-2.60996;,
 -1.10262;-0.59398;-5.51862;,
 -4.47571;-0.25409;-4.70401;,
 -6.50967;-0.06790;-4.31569;,
 -9.14412;0.11932;-3.80144;,
 -9.21605;0.62246;-1.78365;,
 0.90597;4.70355;0.00000;,
 0.90597;4.70021;0.85530;,
 1.47242;4.72786;0.85530;,
 1.47242;4.73119;0.00000;,
 0.90597;4.80713;3.19990;,
 1.47242;4.83476;3.19990;,
 2.46070;4.89626;0.85530;,
 2.46679;4.88594;0.08748;,
 2.28226;5.01937;2.67321;,
 7.07698;2.25550;0.09337;,
 7.18893;2.25550;1.18655;,
 9.98887;1.11138;0.91800;,
 9.95324;1.11138;-0.28365;,
 6.62915;2.25550;3.98281;,
 9.51965;1.11138;4.95297;,
 8.73710;0.29525;-0.29295;,
 8.76347;0.29535;0.55617;,
 6.40980;0.13698;0.54945;,
 6.40724;0.13676;-0.38364;,
 8.43889;0.29576;3.40451;,
 6.41873;0.13794;3.93569;,
 4.60131;-0.02019;0.54328;,
 4.61884;-0.02026;-0.45254;,
 5.03585;-0.01997;4.27613;,
 6.60542;0.88340;5.73092;,
 3.80202;2.25550;4.62559;,
 2.00401;2.25550;4.62559;,
 4.59940;0.65550;6.23672;,
 9.81072;1.12829;-4.62206;,
 6.60544;0.54467;-5.24620;,
 3.80202;2.25550;-4.92215;,
 6.62915;2.25550;-4.27936;,
 4.11369;0.29962;-5.71817;,
 2.00401;2.25550;-4.92215;,
 1.81419;-0.31012;4.49503;,
 1.82263;-0.30932;0.59475;,
 1.82514;-0.30908;-0.55001;,
 1.06504;3.06268;-4.53891;,
 2.86305;3.06268;-4.53891;,
 2.86305;3.06268;4.20214;,
 1.06504;3.06268;4.20214;,
 5.38360;3.06268;0.24882;,
 5.49555;3.06268;1.30139;,
 4.93578;3.06268;-3.96143;,
 4.93578;3.06268;3.62466;,
 0.60361;3.85227;-3.87173;,
 2.07834;3.85227;-3.87173;,
 2.07834;3.85227;3.54009;,
 0.60360;3.85227;3.75168;,
 3.87758;3.85227;0.16809;,
 3.96941;3.85227;1.05214;,
 3.51029;3.85227;-3.36809;,
 3.77839;3.85227;3.24804;,
 0.90598;4.67706;-3.42118;,
 1.33961;4.76136;-3.42118;,
 2.07358;4.95047;-2.69046;,
 4.69071;-0.52354;-3.97849;,
 4.64514;-0.02035;-1.94627;,
 1.82892;-0.30870;-2.26714;,
 1.82678;-0.81217;-4.67141;,
 6.40597;0.13679;-4.11755;,
 6.40340;0.13644;-1.78326;,
 8.63730;0.29527;-3.68130;,
 8.69753;0.29510;-1.56662;,
 6.90904;2.25550;-1.54641;,
 9.89979;1.11138;-2.08612;,
 5.21567;3.06268;-1.33003;,
 3.73984;3.85227;-1.15798;,
 2.47594;4.87046;-1.06423;,
 1.47242;4.73617;-1.28294;,
 0.90597;4.70854;-1.28294;,
 9.29211;0.62263;0.70819;,
 9.26169;0.62256;-0.28855;,
 8.90797;0.62293;4.05303;,
 6.51889;0.43616;4.68492;,
 4.87995;0.24938;5.09220;,
 1.09353;-0.09162;5.34918;,
 1.09963;-0.09100;0.75924;,
 1.10145;-0.09083;-0.58845;,
 1.10263;-0.59398;-5.51862;,
 1.10418;-0.09059;-2.60996;,
 4.47571;-0.25409;-4.70401;,
 6.50967;-0.06790;-4.31569;,
 9.21605;0.62246;-1.78365;,
 9.14412;0.85681;-3.80144;;
 
 184;
 4;0,1,2,3;,
 4;2,1,4,5;,
 4;6,7,1,0;,
 4;1,7,8,4;,
 4;9,10,11,12;,
 4;11,10,13,14;,
 4;15,16,17,18;,
 4;17,16,19,20;,
 4;21,22,16,15;,
 4;16,22,23,19;,
 4;24,25,14,13;,
 4;26,25,24,27;,
 4;28,29,30,31;,
 4;31,30,32,33;,
 4;3,2,34,35;,
 4;34,2,5,36;,
 4;37,38,26,27;,
 4;22,39,40,23;,
 4;41,39,22,21;,
 4;32,42,43,33;,
 4;30,44,45,32;,
 4;26,46,47,25;,
 4;11,48,49,12;,
 4;29,50,44,30;,
 4;25,47,51,14;,
 4;14,51,48,11;,
 4;32,45,52,42;,
 4;38,53,46,26;,
 4;44,54,55,45;,
 4;46,56,57,47;,
 4;48,58,59,49;,
 4;50,60,54,44;,
 4;47,57,61,51;,
 4;51,61,58,48;,
 4;45,55,62,52;,
 4;53,63,56,46;,
 4;54,64,65,55;,
 4;56,5,4,57;,
 4;58,7,6,59;,
 4;60,66,64,54;,
 4;57,4,8,61;,
 4;61,8,7,58;,
 4;55,65,67,62;,
 4;63,36,5,56;,
 4;68,69,70,71;,
 4;72,68,71,73;,
 4;74,72,73,75;,
 4;29,28,76,77;,
 4;78,50,29,77;,
 4;79,60,50,78;,
 4;80,66,60,79;,
 4;64,66,80,81;,
 4;65,64,81,82;,
 4;67,65,82,83;,
 4;84,85,10,9;,
 4;10,85,86,13;,
 4;13,86,87,24;,
 4;24,87,88,27;,
 4;27,88,89,37;,
 4;37,89,90,91;,
 4;91,90,92,93;,
 4;94,95,96,43;,
 4;43,96,97,33;,
 4;33,97,98,31;,
 4;28,99,100,76;,
 4;31,98,99,28;,
 4;18,17,85,84;,
 4;85,17,20,86;,
 4;86,20,19,87;,
 4;87,19,23,88;,
 4;88,23,40,89;,
 4;89,40,39,90;,
 4;90,39,41,92;,
 4;95,70,69,96;,
 4;96,69,68,97;,
 4;97,68,72,98;,
 4;99,74,75,100;,
 4;98,72,74,99;,
 4;101,102,103,104;,
 4;102,105,106,103;,
 4;104,103,107,108;,
 4;103,106,109,107;,
 4;110,111,112,113;,
 4;111,114,115,112;,
 4;116,117,118,119;,
 4;117,120,121,118;,
 4;119,118,122,123;,
 4;118,121,124,122;,
 4;125,115,114,126;,
 4;127,128,125,126;,
 4;129,130,131,132;,
 4;130,133,134,131;,
 4;35,34,102,101;,
 4;34,36,105,102;,
 4;37,128,127,38;,
 4;122,124,135,136;,
 4;123,122,136,137;,
 4;134,133,43,42;,
 4;131,134,138,139;,
 4;127,126,140,141;,
 4;111,110,142,143;,
 4;132,131,139,144;,
 4;126,114,145,140;,
 4;114,111,143,145;,
 4;134,42,52,138;,
 4;38,127,141,53;,
 4;139,138,146,147;,
 4;141,140,148,149;,
 4;143,142,150,151;,
 4;144,139,147,152;,
 4;140,145,153,148;,
 4;145,143,151,153;,
 4;138,52,62,146;,
 4;53,141,149,63;,
 4;147,146,154,155;,
 4;149,148,106,105;,
 4;151,150,108,107;,
 4;152,147,155,156;,
 4;148,153,109,106;,
 4;153,151,107,109;,
 4;146,62,67,154;,
 4;63,149,105,36;,
 4;157,158,159,160;,
 4;161,162,158,157;,
 4;163,164,162,161;,
 4;132,165,166,129;,
 4;167,165,132,144;,
 4;168,167,144,152;,
 4;169,168,152,156;,
 4;155,170,169,156;,
 4;154,171,170,155;,
 4;67,83,171,154;,
 4;113,112,172,173;,
 4;112,115,174,172;,
 4;115,125,175,174;,
 4;125,128,176,175;,
 4;128,37,177,176;,
 4;37,91,178,177;,
 4;91,93,179,178;,
 4;94,43,180,181;,
 4;43,133,182,180;,
 4;133,130,183,182;,
 4;129,166,184,185;,
 4;130,129,185,183;,
 4;173,172,117,116;,
 4;172,174,120,117;,
 4;174,175,121,120;,
 4;175,176,124,121;,
 4;176,177,135,124;,
 4;177,178,136,135;,
 4;178,179,137,136;,
 4;181,180,160,159;,
 4;180,182,157,160;,
 4;182,183,161,157;,
 4;185,184,164,163;,
 4;183,185,163,161;,
 4;92,95,94,93;,
 4;41,70,95,92;,
 4;70,41,21,71;,
 4;71,21,15,73;,
 4;73,15,18,75;,
 4;100,75,18,84;,
 4;76,100,84,9;,
 4;76,9,12,77;,
 4;49,78,77,12;,
 4;59,79,78,49;,
 4;6,80,79,59;,
 4;80,6,0,81;,
 4;81,0,3,82;,
 4;82,3,35,83;,
 4;171,83,35,101;,
 4;170,171,101,104;,
 4;169,170,104,108;,
 4;150,168,169,108;,
 4;142,167,168,150;,
 4;110,165,167,142;,
 4;166,165,110,113;,
 4;166,113,173,184;,
 4;184,173,116,164;,
 4;162,164,116,119;,
 4;158,162,119,123;,
 4;159,158,123,137;,
 4;179,181,159,137;,
 4;93,94,181,179;;
 
 MeshMaterialList {
  1;
  184;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\boss\\boss_UV.jpg";
   }
  }
 }
 MeshNormals {
  214;
  -0.000000;0.611025;-0.791611;,
  -0.000000;-0.999203;0.039911;,
  -0.000000;0.608551;0.793514;,
  -0.000000;0.540224;-0.841521;,
  -0.000000;0.623061;0.782173;,
  -0.100285;0.484187;-0.869199;,
  -0.089291;0.663438;0.742885;,
  -0.306394;0.826178;-0.472813;,
  -0.397129;0.917541;0.020194;,
  -0.291532;0.869107;0.399577;,
  -0.275153;0.813986;-0.511584;,
  -0.368275;0.929577;0.016145;,
  -0.255573;0.899984;0.353144;,
  -0.103512;0.431675;-0.896070;,
  -0.082916;0.751371;0.654650;,
  -0.052012;0.474584;-0.878672;,
  -0.120651;0.607129;0.785390;,
  -0.088151;0.664002;0.742516;,
  -0.091112;0.500790;-0.860760;,
  -0.028810;0.547628;-0.836226;,
  -0.098331;0.626252;-0.773395;,
  -0.201760;0.564380;0.800480;,
  -0.103028;0.602229;0.791648;,
  -0.453278;0.891125;0.020859;,
  -0.343209;0.852617;-0.394020;,
  -0.339813;0.832268;0.438015;,
  -0.000000;0.554009;-0.832511;,
  -0.000000;0.621952;0.783055;,
  -0.011215;0.570795;-0.821016;,
  -0.123928;0.657779;-0.742946;,
  -0.209609;0.562069;0.800089;,
  -0.136242;0.572853;0.808256;,
  -0.533491;0.845776;0.007091;,
  -0.424307;0.826611;-0.369699;,
  -0.412870;0.794751;0.444869;,
  -0.000000;0.577992;-0.816043;,
  -0.000000;0.624166;0.781292;,
  0.045313;0.748417;-0.661678;,
  0.081140;0.996552;-0.017345;,
  0.117089;0.992734;-0.027724;,
  0.013103;0.906202;-0.422642;,
  -0.041386;0.969797;0.240375;,
  0.129033;0.990129;-0.054729;,
  -0.217366;0.975873;-0.020568;,
  -0.274455;0.927668;-0.253194;,
  -0.206545;0.978223;-0.020448;,
  0.000000;0.864595;-0.502469;,
  0.000000;0.999903;-0.013952;,
  0.000000;0.918687;0.394986;,
  -0.000000;-0.999998;0.001963;,
  -0.351451;0.935939;-0.022341;,
  -0.388577;0.920811;-0.033384;,
  -0.446068;0.893778;-0.046735;,
  -0.538738;0.840670;-0.055097;,
  -0.234564;0.971914;-0.019043;,
  0.131652;0.991197;0.014014;,
  0.080464;0.996757;-0.000525;,
  0.000000;0.999995;-0.003085;,
  -0.620670;-0.782030;0.056549;,
  -0.546170;-0.837370;0.022581;,
  -0.363284;-0.905777;0.218158;,
  -0.180106;-0.915899;0.358736;,
  -0.151414;-0.925938;0.345994;,
  -0.027624;-0.981034;0.191855;,
  -0.008893;-0.999771;0.019440;,
  0.059901;-0.997119;0.046544;,
  -0.181106;-0.873734;-0.451431;,
  -0.006763;-0.980167;-0.198060;,
  -0.170862;-0.867368;-0.467418;,
  -0.469808;-0.860928;-0.195150;,
  -0.059778;-0.989359;0.132645;,
  -0.328211;-0.937266;0.117512;,
  -0.304719;-0.952370;0.011752;,
  -0.081374;-0.996683;-0.000649;,
  -0.258177;-0.955765;0.140919;,
  -0.133210;-0.975669;0.174142;,
  -0.076285;-0.989878;0.119672;,
  -0.096225;-0.995357;-0.002440;,
  -0.124724;-0.977953;0.167487;,
  0.029850;-0.993324;0.111430;,
  0.096739;-0.995309;-0.001411;,
  0.136810;-0.985338;0.101940;,
  -0.119899;-0.989614;-0.079303;,
  0.057489;-0.998344;0.001816;,
  -0.099957;-0.992885;-0.064721;,
  -0.287866;-0.957252;0.028317;,
  0.099752;0.486590;-0.867917;,
  0.089291;0.663438;0.742885;,
  0.306292;0.827567;-0.470445;,
  0.397125;0.917542;0.020195;,
  0.291532;0.869107;0.399577;,
  0.275106;0.817045;-0.506708;,
  0.368275;0.929577;0.016146;,
  0.255573;0.899984;0.353144;,
  0.102487;0.436581;-0.893809;,
  0.082916;0.751371;0.654650;,
  0.051115;0.474484;-0.878779;,
  0.120650;0.607129;0.785390;,
  0.088150;0.664002;0.742516;,
  0.089320;0.500629;-0.861042;,
  0.028810;0.547628;-0.836226;,
  0.098331;0.626252;-0.773395;,
  0.201759;0.564380;0.800480;,
  0.103028;0.602229;0.791648;,
  0.453274;0.891127;0.020860;,
  0.343209;0.852617;-0.394020;,
  0.339813;0.832268;0.438015;,
  0.011215;0.570795;-0.821016;,
  0.123928;0.657779;-0.742946;,
  0.209608;0.562070;0.800089;,
  0.136242;0.572853;0.808256;,
  0.533491;0.845776;0.007090;,
  0.424307;0.826611;-0.369699;,
  0.412870;0.794751;0.444869;,
  -0.045313;0.748418;-0.661678;,
  -0.081139;0.996552;-0.017345;,
  -0.117089;0.992734;-0.027724;,
  -0.013103;0.906202;-0.422642;,
  0.041386;0.969797;0.240375;,
  -0.129034;0.990129;-0.054729;,
  0.217366;0.975874;-0.020569;,
  0.274455;0.927668;-0.253194;,
  0.206545;0.978223;-0.020448;,
  0.351451;0.935940;-0.022342;,
  0.388577;0.920811;-0.033384;,
  0.446068;0.893778;-0.046735;,
  0.538738;0.840670;-0.055097;,
  0.234564;0.971914;-0.019042;,
  -0.131652;0.991197;0.014014;,
  -0.080464;0.996757;-0.000525;,
  0.550278;-0.833993;-0.040610;,
  0.546171;-0.837370;0.022581;,
  0.363284;-0.905777;0.218158;,
  0.180106;-0.915899;0.358736;,
  0.151414;-0.925937;0.345997;,
  0.027624;-0.981034;0.191857;,
  0.008893;-0.999771;0.019441;,
  -0.059919;-0.997118;0.046544;,
  0.187962;-0.925742;-0.328134;,
  0.006764;-0.980167;-0.198060;,
  0.211661;-0.956394;-0.201270;,
  0.404198;-0.903475;-0.142680;,
  0.114912;-0.992945;0.029237;,
  0.338802;-0.940634;-0.020499;,
  0.304718;-0.952370;0.011752;,
  0.081374;-0.996683;-0.000649;,
  0.258177;-0.955765;0.140919;,
  0.133210;-0.975669;0.174142;,
  0.121315;-0.988865;0.086191;,
  0.096212;-0.995358;-0.002441;,
  0.124725;-0.977952;0.167489;,
  -0.029849;-0.993324;0.111433;,
  -0.096751;-0.995308;-0.001413;,
  -0.136827;-0.985336;0.101940;,
  0.160224;-0.986506;0.033675;,
  -0.057489;-0.998344;0.001816;,
  0.161666;-0.985663;0.048286;,
  0.311801;-0.948406;-0.057498;,
  -0.000000;-1.000000;0.000265;,
  -0.078363;-0.996925;-0.000235;,
  0.094773;-0.995499;0.000136;,
  -0.095141;-0.995464;0.000872;,
  -0.077762;-0.996972;-0.000085;,
  -0.307861;-0.951390;-0.008861;,
  -0.553475;-0.832697;-0.016786;,
  -0.366020;0.930289;-0.024314;,
  -0.396457;0.917407;-0.034434;,
  -0.446295;0.893664;-0.046757;,
  -0.532732;0.845280;-0.041210;,
  -0.240782;0.970354;-0.020888;,
  0.100516;0.994935;-0.000731;,
  0.070058;0.997489;0.010400;,
  0.000000;0.999856;0.016980;,
  -0.070058;0.997489;0.010400;,
  -0.100516;0.994935;-0.000731;,
  0.240782;0.970354;-0.020888;,
  0.532732;0.845280;-0.041210;,
  0.446290;0.893666;-0.046756;,
  0.396453;0.917409;-0.034433;,
  0.366020;0.930289;-0.024314;,
  0.553474;-0.832697;-0.016787;,
  0.307860;-0.951390;-0.008862;,
  0.077762;-0.996972;-0.000086;,
  0.095128;-0.995465;0.000870;,
  -0.094803;-0.995496;0.000135;,
  0.078345;-0.996926;-0.000235;,
  -0.053140;0.452845;0.890004;,
  -0.160200;0.502723;0.849474;,
  -0.264763;0.544762;0.795698;,
  0.120085;0.992764;0.000171;,
  -0.569757;-0.821468;0.023790;,
  -0.577150;-0.816454;-0.017337;,
  -0.378463;-0.897095;0.228005;,
  -0.183693;-0.909178;0.373700;,
  -0.154573;-0.921508;0.356271;,
  0.000000;-0.977238;0.212147;,
  0.000000;-0.962519;-0.271216;,
  -0.190235;-0.836796;-0.513404;,
  -0.191658;-0.815909;-0.545490;,
  -0.521279;-0.805348;-0.282280;,
  -0.658912;-0.751771;0.026003;,
  0.160200;0.502722;0.849474;,
  0.053140;0.452845;0.890004;,
  0.264763;0.544761;0.795699;,
  -0.120085;0.992764;0.000170;,
  0.577150;-0.816454;-0.017338;,
  0.569759;-0.821467;0.023790;,
  0.378464;-0.897095;0.228005;,
  0.183693;-0.909178;0.373700;,
  0.154573;-0.921509;0.356271;,
  0.195160;-0.830625;-0.521513;,
  0.238778;-0.871823;-0.427679;,
  0.378679;-0.903968;-0.198604;,
  0.516602;-0.855160;-0.042700;;
  184;
  4;170,39,38,171;,
  4;38,39,42,41;,
  4;169,43,39,170;,
  4;39,43,45,42;,
  4;165,11,8,166;,
  4;8,11,12,9;,
  4;162,73,72,163;,
  4;72,73,75,74;,
  4;161,77,73,162;,
  4;73,77,78,75;,
  4;14,6,9,12;,
  4;16,6,14,17;,
  4;10,7,5,13;,
  4;13,5,15,18;,
  4;171,38,47,172;,
  4;47,38,41,48;,
  4;2,4,16,17;,
  4;77,80,79,78;,
  4;160,80,77,161;,
  4;15,3,0,18;,
  4;5,20,19,15;,
  4;16,21,22,6;,
  4;8,23,167,166;,
  4;7,24,20,5;,
  4;6,22,25,9;,
  4;9,25,23,8;,
  4;15,19,26,3;,
  4;4,27,21,16;,
  4;20,29,28,19;,
  4;21,30,31,22;,
  4;23,32,168,167;,
  4;24,33,29,20;,
  4;22,31,34,25;,
  4;25,34,32,23;,
  4;19,28,35,26;,
  4;27,36,30,21;,
  4;29,40,37,28;,
  4;30,186,187,31;,
  4;32,43,169,168;,
  4;33,44,40,29;,
  4;31,187,188,34;,
  4;34,45,43,32;,
  4;28,37,46,35;,
  4;36,48,41,30;,
  4;82,83,81,76;,
  4;84,82,76,70;,
  4;85,84,70,71;,
  4;7,10,50,51;,
  4;52,24,7,51;,
  4;53,33,24,52;,
  4;54,44,33,53;,
  4;40,44,54,55;,
  4;189,40,55,56;,
  4;46,37,56,57;,
  4;164,59,190,191;,
  4;190,59,60,192;,
  4;192,60,61,193;,
  4;193,61,62,194;,
  4;194,62,63,195;,
  4;195,63,64,1;,
  4;1,64,159,158;,
  4;49,65,67,196;,
  4;196,67,66,197;,
  4;197,66,68,198;,
  4;199,69,58,200;,
  4;198,68,69,199;,
  4;163,72,59,164;,
  4;59,72,74,60;,
  4;60,74,75,61;,
  4;61,75,78,62;,
  4;62,78,79,63;,
  4;63,79,80,64;,
  4;64,80,160,159;,
  4;65,81,83,67;,
  4;67,83,82,66;,
  4;66,82,84,68;,
  4;69,85,71,58;,
  4;68,84,85,69;,
  4;173,115,116,174;,
  4;115,118,119,116;,
  4;174,116,120,175;,
  4;116,119,122,120;,
  4;178,89,92,179;,
  4;89,90,93,92;,
  4;181,144,145,182;,
  4;144,146,147,145;,
  4;182,145,149,183;,
  4;145,147,150,149;,
  4;95,93,90,87;,
  4;97,98,95,87;,
  4;91,94,86,88;,
  4;94,99,96,86;,
  4;172,47,115,173;,
  4;47,48,118,115;,
  4;2,98,97,4;,
  4;149,150,151,152;,
  4;183,149,152,184;,
  4;96,99,0,3;,
  4;86,96,100,101;,
  4;97,87,103,102;,
  4;89,178,177,104;,
  4;88,86,101,105;,
  4;87,90,106,103;,
  4;90,89,104,106;,
  4;96,3,26,100;,
  4;4,97,102,27;,
  4;101,100,107,108;,
  4;102,103,110,109;,
  4;104,177,176,111;,
  4;105,101,108,112;,
  4;103,106,113,110;,
  4;106,104,111,113;,
  4;100,26,35,107;,
  4;27,102,109,36;,
  4;108,107,114,117;,
  4;109,110,201,202;,
  4;111,176,175,120;,
  4;112,108,117,121;,
  4;110,113,203,201;,
  4;113,111,120,122;,
  4;107,35,46,114;,
  4;36,109,118,48;,
  4;154,148,153,155;,
  4;156,142,148,154;,
  4;157,143,142,156;,
  4;88,124,123,91;,
  4;125,124,88,105;,
  4;126,125,105,112;,
  4;127,126,112,121;,
  4;117,128,127,121;,
  4;204,129,128,117;,
  4;46,57,129,114;,
  4;205,206,131,180;,
  4;206,207,132,131;,
  4;207,208,133,132;,
  4;208,209,134,133;,
  4;209,195,135,134;,
  4;195,1,136,135;,
  4;1,158,185,136;,
  4;49,196,139,137;,
  4;196,210,138,139;,
  4;210,211,140,138;,
  4;212,213,130,141;,
  4;211,212,141,140;,
  4;180,131,144,181;,
  4;131,132,146,144;,
  4;132,133,147,146;,
  4;133,134,150,147;,
  4;134,135,151,150;,
  4;135,136,152,151;,
  4;136,185,184,152;,
  4;137,139,155,153;,
  4;139,138,154,155;,
  4;138,140,156,154;,
  4;141,130,143,157;,
  4;140,141,157,156;,
  4;159,65,49,158;,
  4;160,81,65,159;,
  4;81,160,161,76;,
  4;76,161,162,70;,
  4;70,162,163,71;,
  4;58,71,163,164;,
  4;200,58,164,191;,
  4;50,165,166,51;,
  4;167,52,51,166;,
  4;168,53,52,167;,
  4;169,54,53,168;,
  4;54,169,170,55;,
  4;55,170,171,56;,
  4;56,171,172,57;,
  4;129,57,172,173;,
  4;128,129,173,174;,
  4;127,128,174,175;,
  4;176,126,127,175;,
  4;177,125,126,176;,
  4;178,124,125,177;,
  4;123,124,178,179;,
  4;213,205,180,130;,
  4;130,180,181,143;,
  4;142,143,181,182;,
  4;148,142,182,183;,
  4;153,148,183,184;,
  4;185,137,153,184;,
  4;158,49,137,185;;
 }
 MeshTextureCoords {
  186;
  0.766621;0.392381;,
  0.766621;0.392517;,
  0.773393;0.393649;,
  0.773393;0.393513;,
  0.766621;0.388139;,
  0.773393;0.389270;,
  0.754732;0.386042;,
  0.754805;0.385620;,
  0.756939;0.380577;,
  0.665228;0.540638;,
  0.664802;0.540638;,
  0.698277;0.493779;,
  0.699615;0.493779;,
  0.670411;0.540638;,
  0.704969;0.493779;,
  0.707622;0.580557;,
  0.707591;0.580548;,
  0.679452;0.574061;,
  0.679767;0.574066;,
  0.707485;0.580509;,
  0.683333;0.574045;,
  0.729003;0.586988;,
  0.729213;0.586985;,
  0.724018;0.586976;,
  0.705253;0.549976;,
  0.738769;0.493779;,
  0.760265;0.493779;,
  0.729236;0.559310;,
  0.666932;0.586639;,
  0.704969;0.493779;,
  0.738769;0.493779;,
  0.705253;0.595978;,
  0.760265;0.493779;,
  0.735043;0.605311;,
  0.784224;0.398086;,
  0.784224;0.397029;,
  0.784224;0.393698;,
  0.784224;0.576181;,
  0.784224;0.493779;,
  0.762434;0.598827;,
  0.762534;0.598860;,
  0.762404;0.598817;,
  0.784224;0.497112;,
  0.784224;0.576998;,
  0.749995;0.460718;,
  0.771491;0.460718;,
  0.771491;0.460718;,
  0.749995;0.460718;,
  0.718522;0.460718;,
  0.719860;0.460718;,
  0.725214;0.460718;,
  0.725214;0.460718;,
  0.784224;0.456453;,
  0.784224;0.448695;,
  0.759377;0.428379;,
  0.777008;0.428379;,
  0.777008;0.428379;,
  0.759377;0.428379;,
  0.736768;0.428379;,
  0.737865;0.428379;,
  0.742257;0.428379;,
  0.739051;0.428379;,
  0.784224;0.416355;,
  0.784224;0.416355;,
  0.768209;0.391145;,
  0.773393;0.394598;,
  0.759434;0.383400;,
  0.784224;0.398225;,
  0.728144;0.607601;,
  0.762384;0.619423;,
  0.762358;0.598802;,
  0.728689;0.586992;,
  0.707637;0.601167;,
  0.707668;0.580570;,
  0.680960;0.594676;,
  0.680240;0.574071;,
  0.665867;0.540638;,
  0.701623;0.493779;,
  0.721868;0.460718;,
  0.739512;0.428379;,
  0.754623;0.386676;,
  0.766621;0.392176;,
  0.773393;0.393308;,
  0.784224;0.395441;,
  0.673496;0.560660;,
  0.673132;0.560657;,
  0.677724;0.560644;,
  0.706287;0.568294;,
  0.725882;0.575944;,
  0.771150;0.589910;,
  0.771078;0.589885;,
  0.784224;0.576181;,
  0.771056;0.589879;,
  0.784224;0.576181;,
  0.784224;0.576181;,
  0.771023;0.589869;,
  0.771042;0.610487;,
  0.730715;0.596565;,
  0.706397;0.588939;,
  0.674901;0.581271;,
  0.674041;0.560664;,
  0.795056;0.393513;,
  0.795056;0.393649;,
  0.801828;0.392517;,
  0.801828;0.392381;,
  0.795056;0.389270;,
  0.801828;0.388139;,
  0.813643;0.385620;,
  0.813716;0.386042;,
  0.811510;0.380577;,
  0.868833;0.493779;,
  0.870172;0.493779;,
  0.903647;0.540638;,
  0.903221;0.540638;,
  0.863479;0.493779;,
  0.898037;0.540638;,
  0.888681;0.574066;,
  0.888996;0.574061;,
  0.860857;0.580548;,
  0.860826;0.580557;,
  0.885116;0.574045;,
  0.860964;0.580509;,
  0.839235;0.586985;,
  0.839445;0.586988;,
  0.844430;0.586976;,
  0.863196;0.549976;,
  0.829679;0.493779;,
  0.808183;0.493779;,
  0.839213;0.559310;,
  0.901517;0.586639;,
  0.863196;0.595978;,
  0.829679;0.493779;,
  0.863479;0.493779;,
  0.833406;0.605311;,
  0.808183;0.493779;,
  0.805914;0.598860;,
  0.806015;0.598827;,
  0.806045;0.598817;,
  0.796957;0.460718;,
  0.818454;0.460718;,
  0.818454;0.460718;,
  0.796957;0.460718;,
  0.848588;0.460718;,
  0.849927;0.460718;,
  0.843234;0.460718;,
  0.843234;0.460718;,
  0.791441;0.428379;,
  0.809072;0.428379;,
  0.809072;0.428379;,
  0.791441;0.428379;,
  0.830583;0.428379;,
  0.831681;0.428379;,
  0.826192;0.428379;,
  0.829397;0.428379;,
  0.795056;0.394598;,
  0.800240;0.391145;,
  0.809015;0.383400;,
  0.840304;0.607601;,
  0.839759;0.586992;,
  0.806090;0.598802;,
  0.806064;0.619423;,
  0.860811;0.580555;,
  0.860780;0.580570;,
  0.887488;0.574065;,
  0.888208;0.574071;,
  0.866826;0.493779;,
  0.902582;0.540638;,
  0.846580;0.460718;,
  0.828936;0.428379;,
  0.813825;0.386676;,
  0.801828;0.392176;,
  0.795056;0.393308;,
  0.895316;0.560657;,
  0.894953;0.560660;,
  0.890724;0.560644;,
  0.862161;0.568294;,
  0.842567;0.575944;,
  0.797298;0.589910;,
  0.797371;0.589885;,
  0.797393;0.589879;,
  0.797407;0.610487;,
  0.797425;0.589869;,
  0.837734;0.596565;,
  0.862051;0.588939;,
  0.894407;0.560664;,
  0.893547;0.581271;;
 }
}