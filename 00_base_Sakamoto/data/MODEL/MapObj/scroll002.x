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
 287;
 -1.61813;-0.14787;0.00000;,
 -1.13593;-0.14787;-1.16413;,
 -1.20412;0.08495;-1.23233;,
 -1.71457;0.08495;0.00000;,
 -1.13593;-0.14787;1.16414;,
 -1.20412;0.08495;1.23233;,
 0.02820;-0.14787;1.64634;,
 0.02820;0.08495;1.74277;,
 1.19234;-0.14787;1.16414;,
 1.26053;0.08495;1.23233;,
 1.67454;-0.14787;0.00000;,
 1.77098;0.08495;0.00000;,
 1.19234;-0.14787;-1.16413;,
 1.26053;0.08495;-1.23233;,
 0.02820;-0.14787;-1.64633;,
 0.02820;0.08495;-1.74277;,
 -1.13593;-0.14787;-1.16413;,
 -1.20412;0.08495;-1.23233;,
 -1.36876;0.18139;-1.39696;,
 -1.94740;0.18139;0.00000;,
 -1.36876;0.18139;1.39696;,
 0.02820;0.18139;1.97560;,
 1.42517;0.18139;1.39696;,
 2.00380;0.18139;0.00000;,
 1.42516;0.18139;-1.39696;,
 0.02820;0.18139;-1.97560;,
 -1.36876;0.18139;-1.39696;,
 -1.53339;0.08495;-1.56159;,
 -2.18022;0.08495;0.00000;,
 -1.53339;0.08495;1.56159;,
 0.02820;0.08495;2.20843;,
 1.58980;0.08495;1.56159;,
 2.23663;0.08495;0.00000;,
 1.58980;0.08495;-1.56159;,
 0.02820;0.08495;-2.20843;,
 -1.53339;0.08495;-1.56159;,
 -1.60158;-0.14787;-1.62978;,
 -2.27666;-0.14787;0.00000;,
 -1.60158;-0.14787;1.62979;,
 0.02820;-0.14787;2.30487;,
 1.65799;-0.14787;1.62979;,
 2.33307;-0.14787;0.00000;,
 1.65799;-0.14787;-1.62978;,
 0.02820;-0.14787;-2.30487;,
 -1.60158;-0.14787;-1.62978;,
 -1.53339;-0.38066;-1.56159;,
 -2.18022;-0.38066;0.00000;,
 -1.53339;-0.38066;1.56159;,
 0.02820;-0.38066;2.20843;,
 1.58980;-0.38066;1.56159;,
 2.23663;-0.38066;0.00000;,
 1.58980;-0.38066;-1.56159;,
 0.02820;-0.38066;-2.20843;,
 -1.53339;-0.38066;-1.56159;,
 -1.36876;-0.47715;-1.39696;,
 -1.94740;-0.47715;0.00000;,
 -1.36876;-0.47715;1.39696;,
 0.02820;-0.47715;1.97560;,
 1.42517;-0.47715;1.39696;,
 2.00380;-0.47715;0.00000;,
 1.42516;-0.47715;-1.39696;,
 0.02820;-0.47715;-1.97560;,
 -1.36876;-0.47715;-1.39696;,
 -1.20412;-0.38066;-1.23233;,
 -1.71457;-0.38066;0.00000;,
 -1.20412;-0.38066;1.23233;,
 0.02820;-0.38066;1.74277;,
 1.26053;-0.38066;1.23233;,
 1.77098;-0.38066;0.00000;,
 1.26053;-0.38066;-1.23233;,
 0.02820;-0.38066;-1.74277;,
 -1.20412;-0.38066;-1.23233;,
 -1.13593;-0.14787;-1.16413;,
 -1.61813;-0.14787;0.00000;,
 -1.13593;-0.14787;1.16414;,
 0.02820;-0.14787;1.64634;,
 1.19234;-0.14787;1.16414;,
 1.67454;-0.14787;0.00000;,
 1.19234;-0.14787;-1.16413;,
 0.02820;-0.14787;-1.64633;,
 -1.13593;-0.14787;-1.16413;,
 1.40198;-6.29857;0.54825;,
 1.29621;-6.29857;-0.19479;,
 1.27045;6.65476;-0.37583;,
 1.37621;6.65476;0.36721;,
 0.87436;6.65476;-0.31945;,
 0.98013;6.65476;0.42359;,
 0.90013;-6.29857;-0.13841;,
 1.00590;-6.29857;0.60463;,
 1.29621;-6.29857;-0.19479;,
 1.40198;-6.29857;0.54825;,
 0.90013;-6.29857;-0.13841;,
 0.87436;6.65476;-0.31945;,
 1.31363;6.64640;-1.21799;,
 1.21027;-6.30692;-1.26195;,
 1.36351;-6.30692;-1.18847;,
 1.45869;6.64640;-1.20202;,
 1.00590;-6.29857;0.60463;,
 0.79261;-6.29857;1.49660;,
 0.68010;-6.29857;1.17423;,
 0.78321;6.65476;1.43058;,
 0.98013;6.65476;0.42359;,
 0.65433;6.65476;0.99319;,
 -0.03863;-6.29857;1.67359;,
 -0.22511;-6.29857;1.24327;,
 -0.33386;6.65476;1.68765;,
 -0.17285;6.65476;1.23583;,
 -0.98878;-6.29857;0.45470;,
 -0.52257;-6.29857;0.70852;,
 -1.37422;6.65476;1.00205;,
 -0.83943;6.65476;0.77337;,
 -1.20134;-6.29857;0.01840;,
 -0.96525;-6.29857;0.05275;,
 -1.25518;6.65476;-0.21289;,
 -0.96525;6.65476;0.05275;,
 -0.78193;-6.29857;-1.20757;,
 -0.64164;-6.29857;-0.70330;,
 -0.78193;6.65476;-1.20757;,
 -0.64164;6.65476;-0.70330;,
 0.47989;-6.29857;-1.55278;,
 0.26111;-6.29857;-1.11239;,
 0.43918;6.65476;-1.34757;,
 0.22040;6.65476;-0.90718;,
 1.17282;-6.29857;-1.20543;,
 0.90045;-6.29857;-0.76659;,
 1.17282;6.65476;-1.20543;,
 0.90045;6.65476;-0.76659;,
 1.75613;-6.29857;-0.55504;,
 1.39078;-6.29857;-0.34006;,
 1.75613;6.65476;-0.55504;,
 1.39078;6.65476;-0.34006;,
 1.82586;-6.29857;0.73567;,
 1.43720;-6.29857;0.40514;,
 1.82586;6.65476;0.73567;,
 1.43720;6.65476;0.40514;,
 1.00196;-6.29857;1.83932;,
 0.79725;-6.29857;1.39404;,
 1.00196;6.65476;1.83932;,
 0.81362;6.65476;1.50906;,
 -0.55584;-6.29857;1.63393;,
 -0.46450;-6.29857;1.38272;,
 -0.40671;6.65476;2.06544;,
 -0.40580;6.65476;1.79509;,
 -1.67543;-6.29722;1.17601;,
 -1.47682;-6.29722;0.94886;,
 -1.67543;6.65609;1.17601;,
 -1.46044;6.65609;1.06389;,
 -1.56637;-6.29510;-0.56030;,
 -1.38007;-6.29510;-0.47662;,
 -1.47119;6.65822;-0.57385;,
 -1.26852;6.65822;-0.37514;,
 -0.88030;-6.29652;-1.69653;,
 -0.87820;-6.29652;-1.46832;,
 -0.78512;6.65682;-1.71008;,
 -0.76664;6.65682;-1.36685;,
 0.62848;-6.30606;-1.61640;,
 0.81770;-6.30606;-1.57793;,
 1.04807;-6.30606;-1.71855;,
 1.14325;6.64727;-1.73210;,
 0.92106;6.64727;-1.53397;,
 0.74003;6.64727;-1.51492;,
 1.53295;-6.30649;-1.36032;,
 1.62813;6.64684;-1.37386;,
 -0.60573;-6.17709;-0.67136;,
 -0.99061;-6.17709;0.11060;,
 -0.99061;6.72575;0.11060;,
 -0.60573;6.72575;-0.67136;,
 -0.61922;-6.17709;0.89905;,
 -0.61922;6.72575;0.89905;,
 0.22878;-6.17709;1.10028;,
 0.22878;6.72575;1.10028;,
 0.91482;-6.17709;0.56275;,
 0.91482;6.72575;0.56275;,
 0.92230;-6.17709;-0.30876;,
 0.92230;6.72575;-0.30876;,
 0.24560;-6.17709;-0.85799;,
 0.24560;6.72575;-0.85799;,
 -0.60573;-6.17709;-0.67136;,
 -0.60573;6.72575;-0.67136;,
 -0.88025;-6.47286;0.10628;,
 -0.53719;-6.47286;-0.59073;,
 0.01495;-6.47286;0.11397;,
 -0.54921;-6.47286;0.80907;,
 0.20666;-6.47286;0.98844;,
 0.81817;-6.47286;0.50931;,
 0.82484;-6.47286;-0.26752;,
 0.22165;-6.47286;-0.75709;,
 -0.53603;7.03495;-0.58936;,
 -0.87838;7.03495;0.10620;,
 0.01497;7.03495;0.11388;,
 -0.54803;7.03495;0.80754;,
 0.20628;7.03495;0.98654;,
 0.81653;7.03495;0.50840;,
 0.82319;7.03495;-0.26682;,
 0.22124;7.03495;-0.75537;,
 -0.60573;6.72575;-0.67136;,
 -0.99061;6.72575;0.11060;,
 -0.61922;6.72575;0.89905;,
 0.22878;6.72575;1.10028;,
 0.91482;6.72575;0.56275;,
 0.92230;6.72575;-0.30876;,
 0.24560;6.72575;-0.85799;,
 -0.99061;-6.17709;0.11060;,
 -0.60573;-6.17709;-0.67136;,
 -0.61922;-6.17709;0.89905;,
 0.22878;-6.17709;1.10028;,
 0.91482;-6.17709;0.56275;,
 0.92230;-6.17709;-0.30876;,
 0.24560;-6.17709;-0.85799;,
 -0.54886;8.00799;-0.60799;,
 -0.54886;6.48465;-0.60799;,
 -0.90391;6.48465;0.11337;,
 -0.90391;8.00799;0.11337;,
 -0.56130;6.48465;0.84072;,
 -0.56130;8.00799;0.84072;,
 0.22097;6.48465;1.02635;,
 0.22097;8.00799;1.02635;,
 0.85385;6.48465;0.53048;,
 0.85385;8.00799;0.53048;,
 0.86075;6.48465;-0.27349;,
 0.86075;8.00799;-0.27349;,
 0.23649;6.48465;-0.78015;,
 0.23649;8.00799;-0.78015;,
 -0.54886;6.48465;-0.60799;,
 -0.54886;8.00799;-0.60799;,
 -0.90391;8.00799;0.11337;,
 0.02257;8.00799;0.12133;,
 -0.54886;8.00799;-0.60799;,
 -0.56130;8.00799;0.84072;,
 0.22097;8.00799;1.02635;,
 0.85385;8.00799;0.53048;,
 0.86075;8.00799;-0.27349;,
 0.23649;8.00799;-0.78015;,
 -0.54886;6.48465;-0.60799;,
 0.02257;6.48465;0.12133;,
 -0.90391;6.48465;0.11337;,
 -0.56130;6.48465;0.84072;,
 0.22097;6.48465;1.02635;,
 0.85385;6.48465;0.53048;,
 0.86075;6.48465;-0.27349;,
 0.23649;6.48465;-0.78015;,
 -0.54886;-6.17315;-0.60799;,
 -0.54886;-7.69649;-0.60799;,
 -0.90391;-7.69649;0.11337;,
 -0.90391;-6.17315;0.11337;,
 -0.56130;-7.69649;0.84072;,
 -0.56130;-6.17315;0.84072;,
 0.22097;-7.69649;1.02635;,
 0.22097;-6.17315;1.02635;,
 0.85385;-7.69649;0.53048;,
 0.85385;-6.17315;0.53048;,
 0.86075;-7.69649;-0.27349;,
 0.86075;-6.17315;-0.27349;,
 0.23649;-7.69649;-0.78015;,
 0.23649;-6.17315;-0.78015;,
 -0.54886;-7.69649;-0.60799;,
 -0.54886;-6.17315;-0.60799;,
 -0.90391;-6.17315;0.11337;,
 0.02257;-6.17315;0.12133;,
 -0.54886;-6.17315;-0.60799;,
 -0.56130;-6.17315;0.84072;,
 0.22097;-6.17315;1.02635;,
 0.85385;-6.17315;0.53048;,
 0.86075;-6.17315;-0.27349;,
 0.23649;-6.17315;-0.78015;,
 -0.54886;-7.69649;-0.60799;,
 0.02257;-7.69649;0.12133;,
 -0.90391;-7.69649;0.11337;,
 -0.56130;-7.69649;0.84072;,
 0.22097;-7.69649;1.02635;,
 0.85385;-7.69649;0.53048;,
 0.86075;-7.69649;-0.27349;,
 0.23649;-7.69649;-0.78015;,
 0.85751;-4.68987;-1.67593;,
 0.85677;-4.68987;-1.75656;,
 0.85677;5.00447;-1.75656;,
 0.85751;5.00447;-1.67593;,
 -0.62341;5.00447;-1.74284;,
 -0.62266;5.00447;-1.66220;,
 -0.62341;-4.68987;-1.74284;,
 -0.62266;-4.68987;-1.66220;,
 0.85677;-4.68987;-1.75656;,
 0.85751;-4.68987;-1.67593;,
 -0.62341;-4.68987;-1.74284;,
 -0.62341;5.00447;-1.74284;,
 -0.62266;-4.68987;-1.66220;,
 -0.62266;5.00447;-1.66220;;
 
 227;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;8,6,7,9;,
 4;10,8,9,11;,
 4;12,10,11,13;,
 4;14,12,13,15;,
 4;16,14,15,17;,
 4;3,2,18,19;,
 4;5,3,19,20;,
 4;7,5,20,21;,
 4;9,7,21,22;,
 4;11,9,22,23;,
 4;13,11,23,24;,
 4;15,13,24,25;,
 4;17,15,25,26;,
 4;19,18,27,28;,
 4;20,19,28,29;,
 4;21,20,29,30;,
 4;22,21,30,31;,
 4;23,22,31,32;,
 4;24,23,32,33;,
 4;25,24,33,34;,
 4;26,25,34,35;,
 4;28,27,36,37;,
 4;29,28,37,38;,
 4;30,29,38,39;,
 4;31,30,39,40;,
 4;32,31,40,41;,
 4;33,32,41,42;,
 4;34,33,42,43;,
 4;35,34,43,44;,
 4;37,36,45,46;,
 4;38,37,46,47;,
 4;39,38,47,48;,
 4;40,39,48,49;,
 4;41,40,49,50;,
 4;42,41,50,51;,
 4;43,42,51,52;,
 4;44,43,52,53;,
 4;46,45,54,55;,
 4;47,46,55,56;,
 4;48,47,56,57;,
 4;49,48,57,58;,
 4;50,49,58,59;,
 4;51,50,59,60;,
 4;52,51,60,61;,
 4;53,52,61,62;,
 4;55,54,63,64;,
 4;56,55,64,65;,
 4;57,56,65,66;,
 4;58,57,66,67;,
 4;59,58,67,68;,
 4;60,59,68,69;,
 4;61,60,69,70;,
 4;62,61,70,71;,
 4;64,63,72,73;,
 4;65,64,73,74;,
 4;66,65,74,75;,
 4;67,66,75,76;,
 4;68,67,76,77;,
 4;69,68,77,78;,
 4;70,69,78,79;,
 4;71,70,79,80;,
 4;81,82,83,84;,
 4;84,83,85,86;,
 4;86,85,87,88;,
 4;88,87,89,90;,
 4;82,91,92,83;,
 4;93,94,95,96;,
 4;97,81,98,99;,
 4;81,84,100,98;,
 4;84,101,102,100;,
 4;101,97,99,102;,
 4;99,98,103,104;,
 4;98,100,105,103;,
 4;100,102,106,105;,
 4;102,99,104,106;,
 4;104,103,107,108;,
 4;103,105,109,107;,
 4;105,106,110,109;,
 4;106,104,108,110;,
 4;108,107,111,112;,
 4;107,109,113,111;,
 4;109,110,114,113;,
 4;110,108,112,114;,
 4;112,111,115,116;,
 4;111,113,117,115;,
 4;113,114,118,117;,
 4;114,112,116,118;,
 4;116,115,119,120;,
 4;115,117,121,119;,
 4;117,118,122,121;,
 4;118,116,120,122;,
 4;120,119,123,124;,
 4;119,121,125,123;,
 4;121,122,126,125;,
 4;122,120,124,126;,
 4;124,123,127,128;,
 4;123,125,129,127;,
 4;125,126,130,129;,
 4;126,124,128,130;,
 4;128,127,131,132;,
 4;127,129,133,131;,
 4;129,130,134,133;,
 4;130,128,132,134;,
 4;132,131,135,136;,
 4;131,133,137,135;,
 4;133,134,138,137;,
 4;134,132,136,138;,
 4;136,135,139,140;,
 4;135,137,141,139;,
 4;137,138,142,141;,
 4;138,136,140,142;,
 4;140,139,143,144;,
 4;139,141,145,143;,
 4;141,142,146,145;,
 4;142,140,144,146;,
 4;144,143,147,148;,
 4;143,145,149,147;,
 4;145,146,150,149;,
 4;146,144,148,150;,
 4;148,147,151,152;,
 4;147,149,153,151;,
 4;149,150,154,153;,
 4;150,148,152,154;,
 3;152,151,155;,
 3;156,155,157;,
 3;155,151,157;,
 4;151,153,158,157;,
 3;159,158,160;,
 3;160,158,154;,
 3;158,153,154;,
 4;154,152,155,160;,
 4;155,156,159,160;,
 4;159,156,94,93;,
 3;95,94,161;,
 3;94,156,161;,
 3;156,157,161;,
 4;157,158,162,161;,
 3;96,162,93;,
 3;93,162,159;,
 3;162,158,159;,
 4;161,162,96,95;,
 4;163,164,165,166;,
 4;164,167,168,165;,
 4;167,169,170,168;,
 4;169,171,172,170;,
 4;171,173,174,172;,
 4;173,175,176,174;,
 4;175,177,178,176;,
 3;179,180,181;,
 3;182,179,181;,
 3;183,182,181;,
 3;184,183,181;,
 3;185,184,181;,
 3;186,185,181;,
 3;180,186,181;,
 3;187,188,189;,
 3;188,190,189;,
 3;190,191,189;,
 3;191,192,189;,
 3;192,193,189;,
 3;193,194,189;,
 3;194,187,189;,
 4;195,196,188,187;,
 4;196,197,190,188;,
 4;197,198,191,190;,
 4;198,199,192,191;,
 4;199,200,193,192;,
 4;200,201,194,193;,
 4;201,195,187,194;,
 4;202,203,180,179;,
 4;204,202,179,182;,
 4;205,204,182,183;,
 4;206,205,183,184;,
 4;207,206,184,185;,
 4;208,207,185,186;,
 4;203,208,186,180;,
 4;209,210,211,212;,
 4;212,211,213,214;,
 4;214,213,215,216;,
 4;216,215,217,218;,
 4;218,217,219,220;,
 4;220,219,221,222;,
 4;222,221,223,224;,
 3;225,226,227;,
 3;228,226,225;,
 3;229,226,228;,
 3;230,226,229;,
 3;231,226,230;,
 3;232,226,231;,
 3;227,226,232;,
 3;233,234,235;,
 3;235,234,236;,
 3;236,234,237;,
 3;237,234,238;,
 3;238,234,239;,
 3;239,234,240;,
 3;240,234,233;,
 4;241,242,243,244;,
 4;244,243,245,246;,
 4;246,245,247,248;,
 4;248,247,249,250;,
 4;250,249,251,252;,
 4;252,251,253,254;,
 4;254,253,255,256;,
 3;257,258,259;,
 3;260,258,257;,
 3;261,258,260;,
 3;262,258,261;,
 3;263,258,262;,
 3;264,258,263;,
 3;259,258,264;,
 3;265,266,267;,
 3;267,266,268;,
 3;268,266,269;,
 3;269,266,270;,
 3;270,266,271;,
 3;271,266,272;,
 3;272,266,265;,
 4;273,274,275,276;,
 4;276,275,277,278;,
 4;278,277,279,280;,
 4;280,279,281,282;,
 4;274,283,284,275;,
 4;285,273,276,286;;
 
 MeshMaterialList {
  3;
  227;
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
  0,
  2,
  0,
  2,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  2,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  2,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  2,
  2,
  0,
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
  2;;
  Material {
   0.640000;0.122353;0.150588;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.049500;0.049500;0.049500;1.000000;;
   5.000000;
   0.240000;0.240000;0.240000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  173;
  0.707107;-0.000023;0.707107;,
  1.000000;-0.000023;0.000000;,
  0.707106;-0.000023;-0.707108;,
  0.000001;-0.000023;-1.000000;,
  -0.707105;-0.000023;-0.707109;,
  -1.000000;-0.000023;0.000000;,
  -0.707106;-0.000023;0.707108;,
  0.000001;-0.000023;1.000000;,
  0.494172;0.715253;0.494173;,
  0.698867;0.715251;0.000000;,
  0.494177;0.715246;-0.494179;,
  0.000001;0.715242;-0.698877;,
  -0.494178;0.715245;-0.494179;,
  -0.698872;0.715247;0.000001;,
  -0.494174;0.715250;0.494175;,
  0.000001;0.715253;0.698865;,
  -0.000004;1.000000;-0.000002;,
  -0.000007;1.000000;0.000000;,
  -0.000004;1.000000;0.000002;,
  0.000000;1.000000;0.000001;,
  0.000001;1.000000;0.000000;,
  -0.000004;1.000000;0.000001;,
  -0.000003;1.000000;0.000002;,
  -0.000001;1.000000;0.000000;,
  -0.494180;0.715245;-0.494177;,
  -0.698874;0.715245;-0.000001;,
  -0.494177;0.715250;0.494173;,
  -0.000001;0.715253;0.698865;,
  0.494175;0.715251;0.494173;,
  0.698869;0.715249;-0.000000;,
  0.494176;0.715249;-0.494175;,
  -0.000001;0.715248;-0.698871;,
  -0.707109;-0.000023;-0.707105;,
  -1.000000;-0.000023;-0.000001;,
  -0.707109;-0.000023;0.707105;,
  -0.000001;-0.000023;1.000000;,
  0.707108;-0.000023;0.707106;,
  1.000000;-0.000023;-0.000001;,
  0.707108;-0.000023;-0.707106;,
  -0.000001;-0.000023;-1.000000;,
  -0.494218;-0.715192;-0.494215;,
  -0.698927;-0.715193;-0.000001;,
  -0.494215;-0.715197;0.494211;,
  -0.000001;-0.715201;0.698919;,
  0.494213;-0.715199;0.494211;,
  0.698923;-0.715197;-0.000000;,
  0.494214;-0.715196;-0.494213;,
  -0.000001;-0.715195;-0.698925;,
  -0.000004;-1.000000;-0.000002;,
  -0.000007;-1.000000;0.000000;,
  -0.000004;-1.000000;0.000002;,
  0.000000;-1.000000;0.000001;,
  0.000001;-1.000000;0.000000;,
  -0.000004;-1.000000;0.000002;,
  -0.000004;-1.000000;0.000002;,
  -0.000001;-1.000000;0.000000;,
  0.494210;-0.715200;0.494211;,
  0.698921;-0.715198;0.000000;,
  0.494215;-0.715193;-0.494217;,
  0.000001;-0.715189;-0.698931;,
  -0.494216;-0.715192;-0.494218;,
  -0.698926;-0.715194;0.000001;,
  -0.494213;-0.715197;0.494213;,
  0.000001;-0.715201;0.698919;,
  0.980209;0.003160;0.197943;,
  0.990021;-0.000000;-0.140920;,
  0.000000;1.000000;0.000000;,
  -0.982168;-0.004581;-0.187949;,
  -0.990020;0.000000;0.140927;,
  0.000000;-1.000000;-0.000000;,
  0.584628;0.005744;0.811281;,
  -0.260640;-0.017492;0.965278;,
  -0.876664;-0.024635;0.480473;,
  -0.994511;-0.008322;-0.104300;,
  -0.634430;0.001355;-0.772979;,
  0.068653;0.007813;-0.997610;,
  0.551252;0.004061;-0.834329;,
  0.923946;0.000000;-0.382523;,
  0.957194;0.000000;0.289448;,
  0.454499;-0.008875;0.890703;,
  -0.000510;-1.000000;-0.000299;,
  -0.240487;-0.013606;0.970557;,
  0.000397;1.000000;0.000260;,
  -0.000725;-0.999999;-0.000989;,
  -0.479254;-0.010397;0.877615;,
  0.000245;1.000000;0.000912;,
  -0.000855;-1.000000;-0.000393;,
  -0.950370;0.004792;-0.311086;,
  0.000548;1.000000;0.000390;,
  -0.003804;-0.999993;0.000325;,
  -0.004266;-0.999967;-0.006889;,
  -0.496438;0.002740;-0.868068;,
  0.004700;0.999979;0.004535;,
  -0.003839;-0.999968;-0.007086;,
  -0.001864;-0.999971;-0.007325;,
  0.307375;-0.003254;-0.951583;,
  0.003876;0.999967;0.007196;,
  0.000155;0.999999;0.001109;,
  -0.000012;-0.999999;-0.001277;,
  -0.460050;0.001782;0.887891;,
  -0.274804;0.001133;0.961500;,
  0.594214;-0.005207;-0.804290;,
  0.000259;-0.999998;-0.001843;,
  -0.607258;-0.171748;-0.775719;,
  -0.983885;-0.178575;-0.008988;,
  -0.619612;-0.184009;0.763035;,
  0.210723;-0.184007;0.960071;,
  0.883028;-0.178572;0.434021;,
  0.891041;-0.171749;-0.420176;,
  0.227580;-0.168694;-0.959036;,
  -0.607733;0.167419;-0.776294;,
  -0.984688;0.174097;-0.008971;,
  -0.620138;0.179423;0.763699;,
  0.210893;0.179426;0.960901;,
  0.883735;0.174101;0.434397;,
  0.891723;0.167421;-0.420476;,
  0.227746;0.164435;-0.959736;,
  -0.616749;0.000000;-0.787160;,
  -0.999963;0.000000;-0.008586;,
  -0.630181;0.000000;0.776448;,
  0.214139;0.000000;0.976803;,
  0.897210;0.000000;0.441604;,
  0.904660;0.000000;-0.426134;,
  0.230889;0.000000;-0.972980;,
  -0.616749;0.000000;-0.787160;,
  -0.999963;0.000000;-0.008586;,
  0.897210;0.000000;0.441604;,
  0.230889;0.000000;-0.972980;,
  0.999958;0.000000;-0.009177;,
  -0.999957;0.000000;0.009300;,
  -0.140909;-0.014116;-0.989922;,
  -0.577733;-0.008725;-0.816180;,
  0.321273;0.000090;-0.946987;,
  0.847420;0.009706;-0.530835;,
  0.999924;0.006387;0.010583;,
  0.679887;-0.003610;0.733308;,
  -0.010483;-0.007632;0.999916;,
  -0.508011;-0.003965;0.861342;,
  -0.896699;0.000000;0.442641;,
  -0.971091;0.001433;-0.238704;,
  -0.538476;0.012954;-0.842541;,
  0.196112;0.018995;-0.980398;,
  0.484244;0.015698;-0.874792;,
  -0.995995;0.003590;-0.089336;,
  0.994971;-0.005778;0.100000;,
  0.959813;-0.008645;0.280508;,
  0.003174;0.999995;-0.000394;,
  0.564682;-0.011328;0.825231;,
  0.003297;0.999988;0.003668;,
  0.025019;-0.006910;0.999663;,
  -0.354886;-0.001465;0.934908;,
  0.000815;-0.999998;-0.001699;,
  0.000590;-0.999997;-0.002436;,
  -0.000254;0.999997;0.002311;,
  0.000070;0.999998;0.001879;,
  -0.000138;0.999997;0.002546;,
  0.712071;-0.004498;0.702093;,
  -0.940001;0.341052;-0.009080;,
  -0.581914;0.328477;-0.743962;,
  -0.590437;0.351031;0.726747;,
  0.200958;0.351037;0.914543;,
  0.843859;0.341059;0.414223;,
  0.854129;0.328482;-0.403192;,
  0.218524;0.322838;-0.920882;,
  -0.580111;-0.336634;-0.741720;,
  -0.936911;-0.349448;-0.009098;,
  -0.588400;-0.359596;0.724207;,
  0.200283;-0.359593;0.911361;,
  0.841113;-0.349443;0.412818;,
  0.851512;-0.336637;-0.401999;,
  0.217883;-0.330880;-0.918175;,
  -0.009269;0.000000;-0.999957;,
  0.009276;0.000000;0.999957;;
  227;
  4;1,0,8,9;,
  4;2,1,9,10;,
  4;3,2,10,11;,
  4;4,3,11,12;,
  4;5,4,12,13;,
  4;6,5,13,14;,
  4;7,6,14,15;,
  4;0,7,15,8;,
  4;9,8,16,17;,
  4;10,9,17,18;,
  4;11,10,18,19;,
  4;12,11,19,20;,
  4;13,12,20,21;,
  4;14,13,21,22;,
  4;15,14,22,23;,
  4;8,15,23,16;,
  4;17,16,24,25;,
  4;18,17,25,26;,
  4;19,18,26,27;,
  4;20,19,27,28;,
  4;21,20,28,29;,
  4;22,21,29,30;,
  4;23,22,30,31;,
  4;16,23,31,24;,
  4;25,24,32,33;,
  4;26,25,33,34;,
  4;27,26,34,35;,
  4;28,27,35,36;,
  4;29,28,36,37;,
  4;30,29,37,38;,
  4;31,30,38,39;,
  4;24,31,39,32;,
  4;33,32,40,41;,
  4;34,33,41,42;,
  4;35,34,42,43;,
  4;36,35,43,44;,
  4;37,36,44,45;,
  4;38,37,45,46;,
  4;39,38,46,47;,
  4;32,39,47,40;,
  4;41,40,48,49;,
  4;42,41,49,50;,
  4;43,42,50,51;,
  4;44,43,51,52;,
  4;45,44,52,53;,
  4;46,45,53,54;,
  4;47,46,54,55;,
  4;40,47,55,48;,
  4;49,48,56,57;,
  4;50,49,57,58;,
  4;51,50,58,59;,
  4;52,51,59,60;,
  4;53,52,60,61;,
  4;54,53,61,62;,
  4;55,54,62,63;,
  4;48,55,63,56;,
  4;57,56,0,1;,
  4;58,57,1,2;,
  4;59,58,2,3;,
  4;60,59,3,4;,
  4;61,60,4,5;,
  4;62,61,5,6;,
  4;63,62,6,7;,
  4;56,63,7,0;,
  4;64,65,65,64;,
  4;66,66,66,66;,
  4;67,68,68,67;,
  4;69,69,69,69;,
  4;130,130,130,130;,
  4;99,99,100,100;,
  4;69,69,69,69;,
  4;64,64,70,70;,
  4;66,66,66,66;,
  4;67,67,131,131;,
  4;69,69,69,69;,
  4;70,70,71,71;,
  4;66,66,66,66;,
  4;131,131,132,132;,
  4;69,69,69,69;,
  4;71,71,72,72;,
  4;66,66,66,66;,
  4;132,132,133,133;,
  4;69,69,69,69;,
  4;72,72,73,73;,
  4;66,66,66,66;,
  4;133,133,134,134;,
  4;69,69,69,69;,
  4;73,73,74,74;,
  4;66,66,66,66;,
  4;134,134,135,135;,
  4;69,69,69,69;,
  4;74,74,75,75;,
  4;66,66,66,66;,
  4;135,135,136,136;,
  4;69,69,69,69;,
  4;75,75,76,76;,
  4;66,66,66,66;,
  4;136,136,137,137;,
  4;69,69,69,69;,
  4;76,76,77,77;,
  4;66,66,66,66;,
  4;137,137,138,138;,
  4;69,69,69,69;,
  4;77,77,78,78;,
  4;66,66,66,66;,
  4;138,138,139,139;,
  4;69,69,69,69;,
  4;78,78,79,79;,
  4;66,66,66,66;,
  4;139,139,140,140;,
  4;69,69,80,80;,
  4;79,79,81,81;,
  4;66,66,82,82;,
  4;140,140,141,141;,
  4;80,80,83,83;,
  4;81,81,84,84;,
  4;82,82,85,85;,
  4;141,141,142,142;,
  4;83,83,86,86;,
  4;143,143,87,87;,
  4;85,85,88,88;,
  4;144,144,145,145;,
  4;86,86,90,89;,
  4;87,87,91,91;,
  4;88,88,92,146;,
  4;145,145,147,147;,
  3;89,90,93;,
  3;98,93,94;,
  3;93,90,94;,
  4;91,91,95,95;,
  3;97,148,96;,
  3;96,148,92;,
  3;148,146,92;,
  4;147,147,149,149;,
  4;149,150,150,149;,
  4;150,150,99,99;,
  3;151,152,102;,
  3;152,98,102;,
  3;98,94,102;,
  4;95,95,101,101;,
  3;153,154,155;,
  3;155,154,97;,
  3;154,148,97;,
  4;156,156,156,156;,
  4;103,104,111,110;,
  4;104,105,112,111;,
  4;105,106,113,112;,
  4;106,107,114,113;,
  4;107,108,115,114;,
  4;108,109,116,115;,
  4;109,103,110,116;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  4;110,111,157,158;,
  4;111,112,159,157;,
  4;112,113,160,159;,
  4;113,114,161,160;,
  4;114,115,162,161;,
  4;115,116,163,162;,
  4;116,110,158,163;,
  4;104,103,164,165;,
  4;105,104,165,166;,
  4;106,105,166,167;,
  4;107,106,167,168;,
  4;108,107,168,169;,
  4;109,108,169,170;,
  4;103,109,170,164;,
  4;117,117,118,118;,
  4;118,118,119,119;,
  4;119,119,120,120;,
  4;120,120,121,121;,
  4;121,121,122,122;,
  4;122,122,123,123;,
  4;123,123,117,117;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  4;124,124,125,125;,
  4;125,125,119,119;,
  4;119,119,120,120;,
  4;120,120,126,126;,
  4;126,126,122,122;,
  4;122,122,127,127;,
  4;127,127,124,124;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  3;69,69,69;,
  4;128,128,128,128;,
  4;66,66,66,66;,
  4;129,129,129,129;,
  4;69,69,69,69;,
  4;171,171,171,171;,
  4;172,172,172,172;;
 }
 MeshTextureCoords {
  287;
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.125000;,
  0.125000;0.125000;,
  0.250000;0.000000;,
  0.250000;0.125000;,
  0.375000;0.000000;,
  0.375000;0.125000;,
  0.500000;0.000000;,
  0.500000;0.125000;,
  0.625000;0.000000;,
  0.625000;0.125000;,
  0.750000;0.000000;,
  0.750000;0.125000;,
  0.875000;0.000000;,
  0.875000;0.125000;,
  1.000000;0.000000;,
  1.000000;0.125000;,
  0.000000;0.250000;,
  0.125000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.000000;0.375000;,
  0.125000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.625000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.000000;0.875000;,
  0.125000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.375000;0.750000;,
  0.625000;0.500000;,
  0.375000;0.500000;,
  0.625000;0.250000;,
  0.375000;0.250000;,
  0.625000;0.000000;,
  0.375000;0.000000;,
  0.875000;1.000000;,
  0.875000;0.750000;,
  0.250000;0.750000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.375000;0.750000;,
  0.125000;1.000000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.750000;,
  0.125000;0.750000;,
  0.125000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.375000;0.750000;,
  0.250000;0.750000;,
  0.125000;0.750000;,
  0.375000;1.000000;,
  0.375000;0.750000;,
  0.375000;0.687500;,
  0.410710;0.687500;,
  0.410710;0.311560;,
  0.375000;0.311560;,
  0.446430;0.687500;,
  0.446430;0.311560;,
  0.482140;0.687500;,
  0.482140;0.311560;,
  0.517860;0.687500;,
  0.517860;0.311560;,
  0.553570;0.687500;,
  0.553570;0.311560;,
  0.589290;0.687500;,
  0.589290;0.311560;,
  0.625000;0.687500;,
  0.625000;0.311560;,
  0.465230;0.996080;,
  0.597420;0.965910;,
  0.500000;0.850000;,
  0.359220;0.911540;,
  0.359220;0.775960;,
  0.465230;0.691420;,
  0.597420;0.721590;,
  0.656250;0.843750;,
  0.597420;0.034090;,
  0.465230;0.003920;,
  0.500000;0.162500;,
  0.359220;0.088460;,
  0.359220;0.224040;,
  0.465230;0.308580;,
  0.597420;0.278410;,
  0.656250;0.156250;,
  0.597420;0.034090;,
  0.465230;0.003920;,
  0.359220;0.088460;,
  0.359220;0.224040;,
  0.465230;0.308580;,
  0.597420;0.278410;,
  0.656250;0.156250;,
  0.465230;0.996080;,
  0.597420;0.965910;,
  0.359220;0.911540;,
  0.359220;0.775960;,
  0.465230;0.691420;,
  0.597420;0.721590;,
  0.656250;0.843750;,
  0.375000;0.687500;,
  0.375000;0.311560;,
  0.410710;0.311560;,
  0.410710;0.687500;,
  0.446430;0.311560;,
  0.446430;0.687500;,
  0.482140;0.311560;,
  0.482140;0.687500;,
  0.517860;0.311560;,
  0.517860;0.687500;,
  0.553570;0.311560;,
  0.553570;0.687500;,
  0.589290;0.311560;,
  0.589290;0.687500;,
  0.625000;0.311560;,
  0.625000;0.687500;,
  0.465230;0.996080;,
  0.500000;0.850000;,
  0.597420;0.965910;,
  0.359220;0.911540;,
  0.359220;0.775960;,
  0.465230;0.691420;,
  0.597420;0.721590;,
  0.656250;0.843750;,
  0.597420;0.034090;,
  0.500000;0.162500;,
  0.465230;0.003920;,
  0.359220;0.088460;,
  0.359220;0.224040;,
  0.465230;0.308580;,
  0.597420;0.278410;,
  0.656250;0.156250;,
  0.375000;0.687500;,
  0.375000;0.311560;,
  0.410710;0.311560;,
  0.410710;0.687500;,
  0.446430;0.311560;,
  0.446430;0.687500;,
  0.482140;0.311560;,
  0.482140;0.687500;,
  0.517860;0.311560;,
  0.517860;0.687500;,
  0.553570;0.311560;,
  0.553570;0.687500;,
  0.589290;0.311560;,
  0.589290;0.687500;,
  0.625000;0.311560;,
  0.625000;0.687500;,
  0.465230;0.996080;,
  0.500000;0.850000;,
  0.597420;0.965910;,
  0.359220;0.911540;,
  0.359220;0.775960;,
  0.465230;0.691420;,
  0.597420;0.721590;,
  0.656250;0.843750;,
  0.597420;0.034090;,
  0.500000;0.162500;,
  0.465230;0.003920;,
  0.359220;0.088460;,
  0.359220;0.224040;,
  0.465230;0.308580;,
  0.597420;0.278410;,
  0.656250;0.156250;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.375000;0.750000;,
  0.625000;0.500000;,
  0.375000;0.500000;,
  0.625000;0.250000;,
  0.375000;0.250000;,
  0.625000;0.000000;,
  0.375000;0.000000;,
  0.875000;1.000000;,
  0.875000;0.750000;,
  0.125000;1.000000;,
  0.125000;0.750000;;
 }
}