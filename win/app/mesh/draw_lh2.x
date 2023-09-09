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
 178;
 -0.01791;0.00893;-0.06976;,
 -0.01530;0.00893;-0.06347;,
 -0.01530;0.00055;-0.06347;,
 -0.01791;0.00055;-0.06976;,
 -0.01530;0.00893;-0.07605;,
 -0.01530;0.00055;-0.07605;,
 -0.00901;0.00893;-0.07866;,
 -0.00901;0.00055;-0.07866;,
 -0.00272;0.00893;-0.07605;,
 -0.00272;0.00055;-0.07605;,
 -0.00011;0.00893;-0.06976;,
 -0.00011;0.00055;-0.06976;,
 0.00000;0.00893;-0.05986;,
 -0.00000;0.00055;-0.05993;,
 -0.00614;0.00893;-0.05205;,
 0.00762;0.00893;-0.05227;,
 0.00762;0.00055;-0.05227;,
 -0.00614;0.00055;-0.05205;,
 -0.01503;-0.00433;-0.05033;,
 -0.01517;0.01601;-0.05033;,
 -0.01679;0.01729;-0.04583;,
 -0.01676;-0.00433;-0.04583;,
 -0.01734;0.01772;-0.04061;,
 -0.01728;-0.00433;-0.04061;,
 -0.01811;0.01630;-0.01397;,
 -0.01793;-0.00433;-0.01397;,
 -0.01756;0.01146;0.00019;,
 -0.01785;-0.00798;-0.00496;,
 -0.01691;0.00367;0.00804;,
 -0.01753;-0.00839;0.00295;,
 0.01772;-0.00886;0.00186;,
 0.01713;0.00395;0.00809;,
 0.01771;0.01146;0.00019;,
 0.01791;-0.00798;-0.00496;,
 0.01836;0.01630;-0.01397;,
 0.01796;-0.00470;-0.01397;,
 0.01738;0.01772;-0.04061;,
 0.01721;-0.00433;-0.04061;,
 0.01691;0.01734;-0.04583;,
 0.01686;-0.00433;-0.04578;,
 0.01548;0.01601;-0.05033;,
 0.01548;-0.00433;-0.05033;,
 0.00955;0.01513;-0.05502;,
 0.00955;-0.00433;-0.05477;,
 -0.00964;0.01497;-0.05473;,
 -0.00964;-0.00433;-0.05472;,
 -0.01127;-0.00163;0.01330;,
 -0.01135;0.00457;0.01259;,
 0.01153;0.00453;0.01261;,
 0.01145;-0.00123;0.01334;,
 -0.00800;0.02101;-0.04733;,
 0.00749;0.02119;-0.04761;,
 0.01199;0.02208;-0.04078;,
 -0.01175;0.02216;-0.04078;,
 0.01260;0.02064;-0.01397;,
 -0.01270;0.02086;-0.01397;,
 0.01261;0.01637;0.00019;,
 -0.01231;0.01644;0.00019;,
 -0.01160;0.01038;0.00878;,
 0.01194;0.01067;0.00838;,
 -0.01517;0.01601;-0.05033;,
 -0.01109;0.01896;-0.04956;,
 -0.01360;0.01971;-0.04583;,
 -0.01679;0.01729;-0.04583;,
 -0.01478;0.02016;-0.04061;,
 -0.01734;0.01772;-0.04061;,
 -0.01560;0.01904;-0.01397;,
 -0.01811;0.01630;-0.01397;,
 -0.01555;0.01495;0.00019;,
 -0.01756;0.01146;0.00019;,
 -0.01509;0.00835;0.00923;,
 -0.01691;0.00367;0.00804;,
 -0.01489;0.00084;0.01234;,
 -0.01488;0.00445;0.01150;,
 0.01713;0.00395;0.00809;,
 0.01518;0.00813;0.00923;,
 0.01564;0.01467;0.00038;,
 0.01771;0.01146;0.00019;,
 0.01584;0.01900;-0.01392;,
 0.01836;0.01630;-0.01397;,
 0.01493;0.02013;-0.04064;,
 0.01738;0.01772;-0.04061;,
 0.01385;0.01959;-0.04573;,
 0.01691;0.01734;-0.04583;,
 0.01080;0.01887;-0.05001;,
 0.01548;0.01601;-0.05033;,
 0.00649;0.01836;-0.05238;,
 0.00955;0.01513;-0.05502;,
 -0.00804;0.01845;-0.05177;,
 -0.00964;0.01497;-0.05473;,
 0.00749;0.02119;-0.04761;,
 -0.00800;0.02101;-0.04733;,
 0.01199;0.02208;-0.04078;,
 -0.01175;0.02216;-0.04078;,
 0.01260;0.02064;-0.01397;,
 -0.01270;0.02086;-0.01397;,
 0.01261;0.01637;0.00019;,
 -0.01231;0.01644;0.00019;,
 0.01506;0.00457;0.01146;,
 0.01508;0.00070;0.01235;,
 -0.01248;-0.00702;-0.05033;,
 -0.01503;-0.00433;-0.05033;,
 -0.01676;-0.00433;-0.04583;,
 -0.01415;-0.00690;-0.04583;,
 -0.01728;-0.00433;-0.04061;,
 -0.01463;-0.00681;-0.04061;,
 -0.01793;-0.00433;-0.01397;,
 -0.01547;-0.00746;-0.01397;,
 -0.01785;-0.00798;-0.00496;,
 -0.01580;-0.01112;-0.00496;,
 -0.01753;-0.00839;0.00295;,
 -0.01568;-0.01212;0.00184;,
 -0.01541;-0.00897;0.00867;,
 -0.01489;0.00084;0.01234;,
 -0.01127;-0.00163;0.01330;,
 -0.01159;-0.00868;0.01020;,
 -0.01223;-0.01317;0.00240;,
 0.01174;-0.00825;0.01055;,
 0.01145;-0.00123;0.01334;,
 0.01508;0.00070;0.01235;,
 0.01558;-0.00895;0.00857;,
 0.01243;-0.01320;0.00220;,
 0.01590;-0.01220;0.00180;,
 0.01772;-0.00886;0.00186;,
 0.01791;-0.00798;-0.00496;,
 0.01607;-0.01126;-0.00495;,
 0.01796;-0.00470;-0.01397;,
 0.01575;-0.00769;-0.01388;,
 0.01721;-0.00433;-0.04061;,
 0.01461;-0.00691;-0.04061;,
 0.01686;-0.00433;-0.04578;,
 0.01432;-0.00696;-0.04606;,
 0.01548;-0.00433;-0.05033;,
 0.01263;-0.00696;-0.05036;,
 0.00955;-0.00433;-0.05477;,
 0.00884;-0.00900;-0.05073;,
 -0.00964;-0.00433;-0.05472;,
 -0.00858;-0.00900;-0.05093;,
 0.01126;-0.00900;-0.04656;,
 -0.01136;-0.00900;-0.04583;,
 0.01199;-0.00900;-0.04061;,
 -0.01188;-0.00900;-0.04061;,
 0.01260;-0.00900;-0.01397;,
 -0.01249;-0.00900;-0.01397;,
 0.01274;-0.01266;-0.00496;,
 -0.01231;-0.01217;-0.00496;,
 -0.01691;0.00367;0.00804;,
 0.01713;0.00395;0.00809;,
 0.00884;-0.00900;-0.05073;,
 -0.00858;-0.00900;-0.05093;,
 -0.01136;-0.00900;-0.04583;,
 0.01126;-0.00900;-0.04656;,
 -0.01188;-0.00900;-0.04061;,
 0.01199;-0.00900;-0.04061;,
 -0.01249;-0.00900;-0.01397;,
 0.01260;-0.00900;-0.01397;,
 -0.01231;-0.01217;-0.00496;,
 0.01274;-0.01266;-0.00496;,
 -0.01530;0.00893;-0.06347;,
 -0.01791;0.00893;-0.06976;,
 -0.00901;0.00893;-0.06976;,
 -0.01530;0.00893;-0.07605;,
 -0.00901;0.00893;-0.07866;,
 -0.00272;0.00893;-0.07605;,
 -0.00011;0.00893;-0.06976;,
 0.00000;0.00893;-0.05986;,
 -0.00614;0.00893;-0.05205;,
 -0.01791;0.00055;-0.06976;,
 -0.01530;0.00055;-0.06347;,
 -0.00901;0.00055;-0.06976;,
 -0.01530;0.00055;-0.07605;,
 -0.00901;0.00055;-0.07866;,
 -0.00272;0.00055;-0.07605;,
 -0.00011;0.00055;-0.06976;,
 -0.00000;0.00055;-0.05993;,
 -0.00614;0.00055;-0.05205;,
 0.00762;0.00893;-0.05227;,
 0.00762;0.00055;-0.05227;;
 
 114;
 4;3,2,1,0;,
 4;5,3,0,4;,
 4;7,5,4,6;,
 4;9,7,6,8;,
 4;11,9,8,10;,
 4;13,11,10,12;,
 4;17,16,15,14;,
 4;2,17,14,1;,
 4;15,16,13,12;,
 4;21,20,19,18;,
 4;23,22,20,21;,
 4;25,24,22,23;,
 4;27,26,24,25;,
 4;29,28,26,27;,
 4;33,32,31,30;,
 4;35,34,32,33;,
 4;37,36,34,35;,
 4;39,38,36,37;,
 4;41,40,38,39;,
 4;43,42,40,41;,
 4;45,44,42,43;,
 4;18,19,44,45;,
 4;49,48,47,46;,
 4;53,52,51,50;,
 4;55,54,52,53;,
 4;57,56,54,55;,
 4;47,48,59,58;,
 4;58,59,56,57;,
 4;63,62,61,60;,
 4;65,64,62,63;,
 4;67,66,64,65;,
 4;69,68,66,67;,
 4;71,70,68,69;,
 4;46,47,73,72;,
 4;77,76,75,74;,
 4;79,78,76,77;,
 4;81,80,78,79;,
 4;83,82,80,81;,
 4;85,84,82,83;,
 4;87,86,84,85;,
 4;89,88,86,87;,
 4;60,61,88,89;,
 4;90,82,84,86;,
 4;91,90,86,88;,
 4;62,91,88,61;,
 4;92,80,82,90;,
 4;64,93,91,62;,
 4;94,78,80,92;,
 4;66,95,93,64;,
 4;96,76,78,94;,
 4;68,97,95,66;,
 4;48,98,75,59;,
 4;70,73,47,58;,
 4;59,75,76,96;,
 4;70,58,97,68;,
 3;72,73,71;,
 3;74,75,98;,
 3;73,70,71;,
 4;99,98,48,49;,
 3;99,74,98;,
 4;103,102,101,100;,
 4;105,104,102,103;,
 4;107,106,104,105;,
 4;109,108,106,107;,
 4;111,110,108,109;,
 4;115,114,113,112;,
 4;116,115,112,111;,
 4;120,119,118,117;,
 4;122,120,117,121;,
 4;125,124,123,122;,
 4;127,126,124,125;,
 4;129,128,126,127;,
 4;131,130,128,129;,
 4;133,132,130,131;,
 4;135,134,132,133;,
 4;137,136,134,135;,
 4;100,101,136,137;,
 4;131,138,135,133;,
 4;139,103,100,137;,
 4;129,140,138,131;,
 4;141,105,103,139;,
 4;127,142,140,129;,
 4;143,107,105,141;,
 4;125,144,142,127;,
 4;145,109,107,143;,
 4;122,121,144,125;,
 4;116,111,109,145;,
 4;112,113,146,110;,
 3;112,110,111;,
 3;123,120,122;,
 4;123,147,119,120;,
 4;117,118,114,115;,
 4;121,117,115,116;,
 4;151,150,149,148;,
 4;153,152,150,151;,
 4;155,154,152,153;,
 4;157,156,154,155;,
 4;121,116,156,157;,
 3;160,159,158;,
 3;160,161,159;,
 3;160,162,161;,
 3;160,163,162;,
 3;160,164,163;,
 3;160,165,164;,
 3;160,158,166;,
 3;169,168,167;,
 3;169,167,170;,
 3;169,170,171;,
 3;169,171,172;,
 3;169,172,173;,
 3;169,173,174;,
 3;169,175,168;,
 4;160,166,176,165;,
 4;177,175,169,174;;
 
 MeshMaterialList {
  2;
  114;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.267000;0.267000;0.267000;1.000000;;
   5.000000;
   0.405000;0.405000;0.405000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.784000;0.784000;0.784000;1.000000;;
   5.000000;
   0.330000;0.330000;0.330000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  126;
  -0.860468;0.000000;0.509505;,
  -1.000000;0.000000;0.000003;,
  -0.707104;0.000000;-0.707110;,
  0.000000;0.000000;-1.000000;,
  0.707104;0.000000;-0.707110;,
  0.979686;-0.000017;-0.200539;,
  0.015908;0.000000;0.999873;,
  -0.425203;0.000000;0.905098;,
  0.921740;0.001655;-0.387804;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -0.811338;-0.003312;-0.584567;,
  -0.973944;-0.003066;-0.226768;,
  -0.997937;-0.003918;-0.064087;,
  -0.999997;-0.002225;-0.001167;,
  -0.999087;0.007682;0.042037;,
  -0.998123;0.014058;0.059603;,
  0.998730;0.011609;0.049026;,
  0.999324;0.003193;0.036618;,
  0.999949;-0.009143;-0.004291;,
  0.998420;-0.008960;-0.055481;,
  0.982086;-0.003124;-0.188410;,
  0.820852;-0.003390;-0.571132;,
  0.317840;-0.006221;-0.948124;,
  -0.336834;-0.004779;-0.941552;,
  -0.123590;0.346082;0.930028;,
  -0.122689;0.142632;0.982142;,
  0.127226;0.353629;0.926693;,
  0.126226;0.145526;0.981269;,
  -0.005463;0.987743;-0.155995;,
  0.000248;0.998609;-0.052718;,
  0.005690;0.984803;0.173582;,
  0.003148;0.898561;0.438837;,
  0.166511;0.679147;0.714866;,
  -0.151619;0.677124;0.720080;,
  -0.379206;0.774206;-0.506762;,
  -0.435892;0.721894;-0.537464;,
  -0.466610;0.820325;-0.330670;,
  -0.579714;0.756103;-0.303710;,
  -0.589296;0.801612;-0.100742;,
  -0.679892;0.729214;-0.077417;,
  -0.639406;0.758503;0.125831;,
  -0.757879;0.643784;0.105647;,
  -0.647763;0.669401;0.363737;,
  -0.832790;0.476558;0.281696;,
  -0.627161;0.497824;0.599033;,
  -0.868175;0.235210;0.436976;,
  0.620145;0.513626;0.592966;,
  0.850946;0.263530;0.454359;,
  0.641919;0.673168;0.367130;,
  0.812139;0.500602;0.299713;,
  0.624417;0.770082;0.130675;,
  0.750984;0.650768;0.111910;,
  0.583226;0.806179;-0.099611;,
  0.680605;0.728631;-0.076633;,
  0.465694;0.820175;-0.332328;,
  0.570896;0.766686;-0.293718;,
  0.360538;0.785394;-0.503159;,
  0.407337;0.748851;-0.522781;,
  0.138574;0.787073;-0.601094;,
  0.150129;0.692822;-0.705308;,
  -0.168200;0.782499;-0.599504;,
  -0.193234;0.674988;-0.712075;,
  0.233391;0.877335;-0.419300;,
  -0.239278;0.880803;-0.408573;,
  0.475743;0.871203;-0.121133;,
  -0.489748;0.863172;-0.122803;,
  0.478713;0.865841;0.145442;,
  -0.503093;0.852393;0.142561;,
  0.430130;0.803559;0.411438;,
  -0.410970;0.810421;0.417519;,
  -0.602694;0.152039;0.783354;,
  0.597997;0.156146;0.786141;,
  -0.605191;0.274167;0.747379;,
  0.596356;0.294147;0.746885;,
  -0.603940;-0.642791;-0.471249;,
  -0.585059;-0.709428;-0.392960;,
  -0.695288;-0.701335;-0.157173;,
  -0.636340;-0.756825;-0.149286;,
  -0.715418;-0.697280;-0.044476;,
  -0.649313;-0.759385;-0.041551;,
  -0.768785;-0.624802;-0.136352;,
  -0.626012;-0.762572;-0.163075;,
  -0.833541;-0.535760;-0.134799;,
  -0.608714;-0.768999;-0.195212;,
  -0.930087;-0.092431;0.355519;,
  -0.905837;-0.372126;0.202440;,
  -0.612056;-0.775494;0.154907;,
  -0.191725;-0.366135;0.910597;,
  -0.161654;-0.655215;0.737944;,
  -0.136304;-0.971808;0.192382;,
  0.202574;-0.360653;0.910436;,
  0.162431;-0.651898;0.740706;,
  0.139016;-0.968434;0.206905;,
  0.936540;-0.100130;0.335955;,
  0.913258;-0.359425;0.191762;,
  0.625254;-0.762185;0.167724;,
  0.850522;-0.509843;-0.129124;,
  0.635008;-0.749494;-0.187146;,
  0.791701;-0.597087;-0.129219;,
  0.634998;-0.754333;-0.166611;,
  0.734463;-0.677454;-0.040259;,
  0.647758;-0.760817;-0.039584;,
  0.702807;-0.697510;-0.139792;,
  0.634518;-0.759100;-0.145442;,
  0.589635;-0.658056;-0.468286;,
  0.571625;-0.717760;-0.397574;,
  0.236361;-0.632173;-0.737896;,
  0.347464;-0.723531;-0.596465;,
  -0.247619;-0.627576;-0.738128;,
  -0.359261;-0.723121;-0.589938;,
  0.560786;-0.814285;-0.149861;,
  -0.573187;-0.807305;-0.140413;,
  0.551600;-0.833228;-0.038335;,
  -0.577836;-0.815254;-0.038282;,
  0.444042;-0.874349;-0.195806;,
  -0.456433;-0.870647;-0.183422;,
  0.349671;-0.909393;-0.225245;,
  -0.317899;-0.918689;-0.234416;,
  -0.710665;-0.231666;0.664294;,
  -0.669983;-0.501465;0.547408;,
  0.675559;-0.494017;0.547327;,
  0.724924;-0.228590;0.649794;,
  -0.004573;-0.983629;-0.180145;,
  -0.010054;-0.972765;-0.231576;,
  0.707115;0.003098;-0.707092;;
  114;
  4;1,0,0,1;,
  4;2,1,1,2;,
  4;3,2,2,3;,
  4;4,3,3,4;,
  4;5,4,4,5;,
  4;8,5,5,8;,
  4;7,6,6,7;,
  4;0,7,7,0;,
  4;125,125,8,8;,
  4;12,12,11,11;,
  4;13,13,12,12;,
  4;14,14,13,13;,
  4;15,15,14,14;,
  4;16,16,15,15;,
  4;18,18,17,17;,
  4;19,19,18,18;,
  4;20,20,19,19;,
  4;21,21,20,20;,
  4;22,22,21,21;,
  4;23,23,22,22;,
  4;24,24,23,23;,
  4;11,11,24,24;,
  4;28,27,25,26;,
  4;30,30,29,29;,
  4;31,31,30,30;,
  4;32,32,31,31;,
  4;25,27,33,34;,
  4;34,33,32,32;,
  4;38,37,35,36;,
  4;40,39,37,38;,
  4;42,41,39,40;,
  4;44,43,41,42;,
  4;46,45,43,44;,
  4;26,25,73,71;,
  4;50,49,47,48;,
  4;52,51,49,50;,
  4;54,53,51,52;,
  4;56,55,53,54;,
  4;58,57,55,56;,
  4;60,59,57,58;,
  4;62,61,59,60;,
  4;36,35,61,62;,
  4;63,55,57,59;,
  4;64,63,59,61;,
  4;37,64,61,35;,
  4;65,53,55,63;,
  4;39,66,64,37;,
  4;67,51,53,65;,
  4;41,68,66,39;,
  4;69,49,51,67;,
  4;43,70,68,41;,
  4;27,74,47,33;,
  4;45,73,25,34;,
  4;33,47,49,69;,
  4;45,34,70,43;,
  3;71,73,46;,
  3;48,47,74;,
  3;73,45,46;,
  4;72,74,27,28;,
  3;72,48,74;,
  4;78,77,75,76;,
  4;80,79,77,78;,
  4;82,81,79,80;,
  4;84,83,81,82;,
  4;87,86,83,84;,
  4;89,88,119,120;,
  4;90,89,120,87;,
  4;121,122,91,92;,
  4;96,121,92,93;,
  4;98,97,95,96;,
  4;100,99,97,98;,
  4;102,101,99,100;,
  4;104,103,101,102;,
  4;106,105,103,104;,
  4;108,107,105,106;,
  4;110,109,107,108;,
  4;76,75,109,110;,
  4;104,111,108,106;,
  4;112,78,76,110;,
  4;102,113,111,104;,
  4;114,80,78,112;,
  4;100,115,113,102;,
  4;116,82,80,114;,
  4;98,117,115,100;,
  4;118,84,82,116;,
  4;96,93,117,98;,
  4;90,87,84,118;,
  4;120,119,85,86;,
  3;120,86,87;,
  3;95,121,96;,
  4;95,94,122,121;,
  4;92,91,88,89;,
  4;93,92,89,90;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;123,123,10,10;,
  4;124,124,123,123;,
  4;93,90,124,124;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  4;9,9,9,9;,
  4;10,10,10,10;;
 }
}
