#ifndef _uvdisplay_H_
#define _uvdisplay_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "ssd_utils.h"
#include "cpld_utils.h"





#define CIRCLE_BMP_DATA_ROW	1000
const uint16_t		exposure_bmp_data_circle[CIRCLE_BMP_DATA_ROW][3] =
{
	720,720,780,
	690,750,781,
	681,759,782,
	673,767,783,
	665,775,784,
	656,784,785,
	648,792,786,
	639,801,787,
	632,808,788,
	627,813,789,
	623,817,790,
	619,821,791,
	615,825,792,
	611,829,793,
	607,833,794,
	603,837,795,
	598,842,796,
	594,846,797,
	590,850,798,
	586,854,799,
	582,858,800,
	578,862,801,
	574,866,802,
	571,869,803,
	569,871,804,
	566,874,805,
	563,877,806,
	560,880,807,
	558,882,808,
	555,885,809,
	552,888,810,
	550,890,811,
	547,893,812,
	544,896,813,
	542,898,814,
	539,901,815,
	536,904,816,
	534,906,817,
	531,909,818,
	528,912,819,
	525,915,820,
	523,917,821,
	520,920,822,
	518,922,823,
	516,924,824,
	514,926,825,
	512,928,826,
	510,930,827,
	508,932,828,
	506,934,829,
	504,936,830,
	502,938,831,
	500,940,832,
	498,942,833,
	496,944,834,
	494,946,835,
	492,948,836,
	491,949,837,
	489,951,838,
	487,953,839,
	485,955,840,
	483,957,841,
	481,959,842,
	479,961,843,
	477,963,844,
	475,965,845,
	473,967,846,
	471,969,847,
	469,971,848,
	467,973,849,
	465,975,850,
	464,976,851,
	462,978,852,
	461,979,853,
	460,980,854,
	458,982,855,
	457,983,856,
	455,985,857,
	454,986,858,
	452,988,859,
	451,989,860,
	449,991,861,
	448,992,862,
	446,994,863,
	445,995,864,
	443,997,865,
	442,998,866,
	440,1000,867,
	439,1001,868,
	437,1003,869,
	436,1004,870,
	434,1006,871,
	433,1007,872,
	431,1009,873,
	430,1010,874,
	428,1012,875,
	427,1013,876,
	425,1015,877,
	424,1016,878,
	422,1018,879,
	421,1019,880,
	419,1021,881,
	418,1022,882,
	417,1023,883,
	415,1025,884,
	414,1026,885,
	413,1027,886,
	412,1028,887,
	411,1029,888,
	410,1030,889,
	408,1032,890,
	407,1033,891,
	406,1034,892,
	405,1035,893,
	404,1036,894,
	403,1037,895,
	402,1038,896,
	400,1040,897,
	399,1041,898,
	398,1042,899,
	397,1043,900,
	396,1044,901,
	395,1045,902,
	393,1047,903,
	392,1048,904,
	391,1049,905,
	390,1050,906,
	389,1051,907,
	388,1052,908,
	386,1054,909,
	385,1055,910,
	384,1056,911,
	383,1057,912,
	382,1058,913,
	381,1059,914,
	379,1061,915,
	378,1062,916,
	377,1063,917,
	376,1064,918,
	375,1065,919,
	374,1066,920,
	372,1067,921,
	372,1068,922,
	371,1069,923,
	370,1070,924,
	369,1071,925,
	368,1072,926,
	367,1073,927,
	366,1074,928,
	365,1075,929,
	364,1076,930,
	363,1077,931,
	362,1078,932,
	361,1079,933,
	361,1079,934,
	360,1080,935,
	359,1081,936,
	358,1082,937,
	357,1083,938,
	356,1084,939,
	355,1085,940,
	354,1086,941,
	353,1087,942,
	352,1088,943,
	351,1089,944,
	350,1090,945,
	350,1090,946,
	349,1091,947,
	348,1092,948,
	347,1093,949,
	346,1094,950,
	345,1095,951,
	344,1096,952,
	343,1097,953,
	342,1098,954,
	341,1099,955,
	340,1100,956,
	339,1100,957,
	339,1101,958,
	338,1102,959,
	337,1103,960,
	336,1104,961,
	335,1105,962,
	334,1106,963,
	333,1107,964,
	332,1108,965,
	331,1108,966,
	331,1109,967,
	330,1110,968,
	329,1111,969,
	329,1111,970,
	328,1112,971,
	327,1113,972,
	326,1114,973,
	326,1114,974,
	325,1115,975,
	324,1116,976,
	324,1116,977,
	323,1117,978,
	322,1118,979,
	321,1119,980,
	321,1119,981,
	320,1120,982,
	319,1121,983,
	319,1121,984,
	318,1122,985,
	317,1123,986,
	316,1124,987,
	316,1124,988,
	315,1125,989,
	314,1126,990,
	314,1126,991,
	313,1127,992,
	312,1128,993,
	311,1129,994,
	311,1129,995,
	310,1130,996,
	309,1131,997,
	309,1131,998,
	308,1132,999,
	307,1133,1000,
	306,1134,1001,
	306,1134,1002,
	305,1135,1003,
	304,1136,1004,
	303,1136,1005,
	303,1137,1006,
	302,1138,1007,
	301,1139,1008,
	301,1139,1009,
	300,1140,1010,
	299,1141,1011,
	298,1142,1012,
	298,1142,1013,
	297,1143,1014,
	297,1143,1015,
	296,1144,1016,
	296,1144,1017,
	295,1145,1018,
	294,1146,1019,
	294,1146,1020,
	293,1147,1021,
	293,1147,1022,
	292,1148,1023,
	292,1148,1024,
	291,1149,1025,
	291,1149,1026,
	290,1150,1027,
	289,1151,1028,
	289,1151,1029,
	288,1152,1030,
	288,1152,1031,
	287,1153,1032,
	287,1153,1033,
	286,1154,1034,
	286,1154,1035,
	285,1155,1036,
	285,1155,1037,
	284,1156,1038,
	283,1157,1039,
	283,1157,1040,
	282,1158,1041,
	282,1158,1042,
	281,1159,1043,
	281,1159,1044,
	280,1160,1045,
	280,1160,1046,
	279,1161,1047,
	278,1162,1048,
	278,1162,1049,
	277,1163,1050,
	277,1163,1051,
	276,1164,1052,
	276,1164,1053,
	275,1165,1054,
	275,1165,1055,
	274,1166,1056,
	273,1167,1057,
	273,1167,1058,
	272,1168,1059,
	272,1168,1060,
	271,1169,1061,
	271,1169,1062,
	270,1170,1063,
	270,1170,1064,
	269,1171,1065,
	269,1171,1066,
	268,1172,1067,
	268,1172,1068,
	267,1173,1069,
	267,1173,1070,
	267,1173,1071,
	266,1174,1072,
	266,1174,1073,
	265,1175,1074,
	265,1175,1075,
	265,1175,1076,
	264,1176,1077,
	264,1176,1078,
	263,1177,1079,
	263,1177,1080,
	263,1177,1081,
	262,1178,1082,
	262,1178,1083,
	261,1179,1084,
	261,1179,1085,
	261,1179,1086,
	260,1180,1087,
	260,1180,1088,
	259,1181,1089,
	259,1181,1090,
	259,1181,1091,
	258,1182,1092,
	258,1182,1093,
	257,1183,1094,
	257,1183,1095,
	257,1183,1096,
	256,1184,1097,
	256,1184,1098,
	255,1185,1099,
	255,1185,1100,
	254,1186,1101,
	254,1186,1102,
	254,1186,1103,
	253,1187,1104,
	253,1187,1105,
	252,1188,1106,
	252,1188,1107,
	252,1188,1108,
	251,1189,1109,
	251,1189,1110,
	250,1190,1111,
	250,1190,1112,
	250,1190,1113,
	249,1191,1114,
	249,1191,1115,
	248,1192,1116,
	248,1192,1117,
	248,1192,1118,
	247,1193,1119,
	247,1193,1120,
	247,1193,1121,
	246,1194,1122,
	246,1194,1123,
	246,1194,1124,
	245,1195,1125,
	245,1195,1126,
	245,1195,1127,
	245,1195,1128,
	244,1196,1129,
	244,1196,1130,
	244,1196,1131,
	244,1196,1132,
	243,1197,1133,
	243,1197,1134,
	243,1197,1135,
	242,1198,1136,
	242,1198,1137,
	242,1198,1138,
	242,1198,1139,
	241,1199,1140,
	241,1199,1141,
	241,1199,1142,
	241,1199,1143,
	240,1200,1144,
	240,1200,1145,
	240,1200,1146,
	239,1201,1147,
	239,1201,1148,
	239,1201,1149,
	239,1201,1150,
	238,1202,1151,
	238,1202,1152,
	238,1202,1153,
	238,1202,1154,
	237,1203,1155,
	237,1203,1156,
	237,1203,1157,
	236,1204,1158,
	236,1204,1159,
	236,1204,1160,
	236,1204,1161,
	235,1205,1162,
	235,1205,1163,
	235,1205,1164,
	234,1205,1165,
	234,1206,1166,
	234,1206,1167,
	234,1206,1168,
	233,1207,1169,
	233,1207,1170,
	233,1207,1171,
	233,1207,1172,
	232,1208,1173,
	232,1208,1174,
	232,1208,1175,
	231,1208,1176,
	231,1209,1177,
	231,1209,1178,
	231,1209,1179,
	231,1209,1180,
	231,1209,1181,
	230,1210,1182,
	230,1210,1183,
	230,1210,1184,
	230,1210,1185,
	230,1210,1186,
	230,1210,1187,
	230,1210,1188,
	229,1211,1189,
	229,1211,1190,
	229,1211,1191,
	229,1211,1192,
	229,1211,1193,
	229,1211,1194,
	229,1211,1195,
	228,1212,1196,
	228,1212,1197,
	228,1212,1198,
	228,1212,1199,
	228,1212,1200,
	228,1212,1201,
	227,1213,1202,
	227,1213,1203,
	227,1213,1204,
	227,1213,1205,
	227,1213,1206,
	227,1213,1207,
	227,1213,1208,
	226,1214,1209,
	226,1214,1210,
	226,1214,1211,
	226,1214,1212,
	226,1214,1213,
	226,1214,1214,
	226,1214,1215,
	225,1215,1216,
	225,1215,1217,
	225,1215,1218,
	225,1215,1219,
	225,1215,1220,
	225,1215,1221,
	224,1215,1222,
	224,1216,1223,
	224,1216,1224,
	224,1216,1225,
	224,1216,1226,
	224,1216,1227,
	224,1216,1228,
	223,1217,1229,
	223,1217,1230,
	223,1217,1231,
	223,1217,1232,
	223,1217,1233,
	223,1217,1234,
	223,1217,1235,
	222,1218,1236,
	222,1218,1237,
	222,1218,1238,
	222,1218,1239,
	222,1218,1240,
	222,1218,1241,
	222,1218,1242,
	222,1218,1243,
	222,1218,1244,
	222,1218,1245,
	222,1218,1246,
	222,1218,1247,
	222,1218,1248,
	222,1218,1249,
	222,1218,1250,
	222,1218,1251,
	222,1218,1252,
	222,1218,1253,
	222,1218,1254,
	222,1218,1255,
	222,1218,1256,
	222,1218,1257,
	222,1218,1258,
	222,1218,1259,
	222,1218,1260,
	222,1218,1261,
	222,1218,1262,
	222,1218,1263,
	222,1218,1264,
	222,1218,1265,
	222,1218,1266,
	222,1218,1267,
	222,1218,1268,
	221,1219,1269,
	221,1219,1270,
	221,1219,1271,
	221,1219,1272,
	221,1219,1273,
	221,1219,1274,
	221,1219,1275,
	221,1219,1276,
	221,1219,1277,
	221,1219,1278,
	221,1219,1279,
	221,1219,1280,
	221,1219,1281,
	221,1219,1282,
	221,1219,1283,
	221,1219,1284,
	221,1219,1285,
	221,1219,1286,
	221,1219,1287,
	221,1219,1288,
	221,1219,1289,
	221,1219,1290,
	221,1219,1291,
	221,1219,1292,
	221,1219,1293,
	221,1219,1294,
	221,1219,1295,
	221,1219,1296,
	221,1219,1297,
	221,1219,1298,
	221,1219,1299,
	221,1219,1300,
	221,1219,1301,
	221,1219,1302,
	221,1219,1303,
	222,1218,1304,
	222,1218,1305,
	222,1218,1306,
	222,1218,1307,
	222,1218,1308,
	222,1218,1309,
	222,1218,1310,
	222,1218,1311,
	222,1218,1312,
	222,1218,1313,
	222,1218,1314,
	223,1217,1315,
	223,1217,1316,
	223,1217,1317,
	223,1217,1318,
	223,1217,1319,
	223,1217,1320,
	223,1217,1321,
	223,1217,1322,
	223,1217,1323,
	223,1217,1324,
	223,1217,1325,
	223,1216,1326,
	224,1216,1327,
	224,1216,1328,
	224,1216,1329,
	224,1216,1330,
	224,1216,1331,
	224,1216,1332,
	224,1216,1333,
	224,1216,1334,
	224,1216,1335,
	224,1216,1336,
	224,1216,1337,
	225,1215,1338,
	225,1215,1339,
	225,1215,1340,
	225,1215,1341,
	225,1215,1342,
	225,1215,1343,
	225,1215,1344,
	225,1215,1345,
	225,1215,1346,
	225,1215,1347,
	225,1215,1348,
	226,1214,1349,
	226,1214,1350,
	226,1214,1351,
	226,1214,1352,
	226,1214,1353,
	226,1214,1354,
	226,1214,1355,
	226,1214,1356,
	227,1213,1357,
	227,1213,1358,
	227,1213,1359,
	227,1213,1360,
	227,1213,1361,
	228,1212,1362,
	228,1212,1363,
	228,1212,1364,
	228,1212,1365,
	229,1211,1366,
	229,1211,1367,
	229,1211,1368,
	229,1211,1369,
	229,1211,1370,
	230,1210,1371,
	230,1210,1372,
	230,1210,1373,
	230,1210,1374,
	230,1210,1375,
	231,1209,1376,
	231,1209,1377,
	231,1209,1378,
	231,1209,1379,
	231,1209,1380,
	232,1208,1381,
	232,1208,1382,
	232,1208,1383,
	232,1208,1384,
	233,1207,1385,
	233,1207,1386,
	233,1207,1387,
	233,1207,1388,
	233,1207,1389,
	234,1206,1390,
	234,1206,1391,
	234,1206,1392,
	234,1206,1393,
	234,1206,1394,
	235,1205,1395,
	235,1205,1396,
	235,1205,1397,
	235,1205,1398,
	235,1205,1399,
	236,1204,1400,
	236,1204,1401,
	236,1204,1402,
	236,1204,1403,
	237,1203,1404,
	237,1203,1405,
	237,1203,1406,
	237,1203,1407,
	237,1203,1408,
	238,1202,1409,
	238,1202,1410,
	238,1202,1411,
	238,1202,1412,
	239,1201,1413,
	239,1201,1414,
	239,1201,1415,
	240,1200,1416,
	240,1200,1417,
	240,1200,1418,
	241,1199,1419,
	241,1199,1420,
	241,1199,1421,
	242,1198,1422,
	242,1198,1423,
	242,1198,1424,
	243,1197,1425,
	243,1197,1426,
	243,1197,1427,
	244,1196,1428,
	244,1196,1429,
	244,1196,1430,
	245,1195,1431,
	245,1195,1432,
	245,1195,1433,
	246,1194,1434,
	246,1194,1435,
	246,1194,1436,
	247,1193,1437,
	247,1193,1438,
	247,1193,1439,
	248,1192,1440,
	248,1192,1441,
	248,1192,1442,
	249,1191,1443,
	249,1191,1444,
	249,1191,1445,
	250,1190,1446,
	250,1190,1447,
	250,1190,1448,
	251,1189,1449,
	251,1189,1450,
	251,1189,1451,
	252,1188,1452,
	252,1188,1453,
	252,1188,1454,
	253,1187,1455,
	253,1187,1456,
	253,1187,1457,
	254,1186,1458,
	254,1186,1459,
	254,1186,1460,
	255,1185,1461,
	255,1185,1462,
	255,1185,1463,
	256,1184,1464,
	256,1184,1465,
	256,1183,1466,
	257,1183,1467,
	257,1183,1468,
	258,1182,1469,
	258,1182,1470,
	259,1181,1471,
	259,1181,1472,
	260,1180,1473,
	260,1180,1474,
	261,1179,1475,
	261,1179,1476,
	261,1179,1477,
	262,1178,1478,
	262,1178,1479,
	263,1177,1480,
	263,1177,1481,
	264,1176,1482,
	264,1176,1483,
	265,1175,1484,
	265,1175,1485,
	266,1174,1486,
	266,1174,1487,
	267,1173,1488,
	267,1173,1489,
	268,1172,1490,
	268,1172,1491,
	269,1171,1492,
	269,1171,1493,
	270,1170,1494,
	270,1170,1495,
	271,1169,1496,
	271,1169,1497,
	271,1169,1498,
	272,1168,1499,
	272,1168,1500,
	273,1167,1501,
	273,1167,1502,
	274,1166,1503,
	274,1166,1504,
	275,1165,1505,
	275,1165,1506,
	276,1164,1507,
	276,1164,1508,
	277,1163,1509,
	277,1163,1510,
	278,1162,1511,
	278,1162,1512,
	279,1161,1513,
	279,1161,1514,
	280,1160,1515,
	280,1160,1516,
	281,1159,1517,
	281,1159,1518,
	281,1158,1519,
	282,1158,1520,
	282,1158,1521,
	283,1157,1522,
	284,1156,1523,
	284,1156,1524,
	285,1155,1525,
	286,1154,1526,
	286,1154,1527,
	287,1153,1528,
	287,1153,1529,
	288,1152,1530,
	289,1151,1531,
	289,1151,1532,
	290,1150,1533,
	291,1149,1534,
	291,1149,1535,
	292,1148,1536,
	292,1148,1537,
	293,1147,1538,
	294,1146,1539,
	294,1146,1540,
	295,1145,1541,
	296,1144,1542,
	296,1144,1543,
	297,1143,1544,
	298,1142,1545,
	298,1142,1546,
	299,1141,1547,
	299,1141,1548,
	300,1140,1549,
	301,1139,1550,
	301,1139,1551,
	302,1138,1552,
	303,1137,1553,
	303,1137,1554,
	304,1136,1555,
	304,1136,1556,
	305,1135,1557,
	306,1134,1558,
	306,1134,1559,
	307,1133,1560,
	308,1132,1561,
	308,1132,1562,
	309,1131,1563,
	310,1130,1564,
	310,1130,1565,
	311,1129,1566,
	311,1129,1567,
	312,1128,1568,
	313,1127,1569,
	313,1127,1570,
	314,1126,1571,
	315,1125,1572,
	315,1125,1573,
	316,1124,1574,
	317,1123,1575,
	318,1122,1576,
	319,1121,1577,
	320,1120,1578,
	320,1120,1579,
	321,1119,1580,
	322,1118,1581,
	323,1117,1582,
	324,1116,1583,
	324,1116,1584,
	325,1115,1585,
	326,1114,1586,
	327,1113,1587,
	328,1112,1588,
	328,1112,1589,
	329,1111,1590,
	330,1110,1591,
	331,1109,1592,
	332,1108,1593,
	333,1107,1594,
	333,1107,1595,
	334,1106,1596,
	335,1105,1597,
	336,1104,1598,
	337,1103,1599,
	337,1103,1600,
	338,1102,1601,
	339,1101,1602,
	340,1100,1603,
	341,1099,1604,
	341,1099,1605,
	342,1098,1606,
	343,1097,1607,
	344,1096,1608,
	345,1095,1609,
	346,1094,1610,
	346,1094,1611,
	347,1093,1612,
	348,1092,1613,
	349,1091,1614,
	350,1090,1615,
	350,1090,1616,
	351,1089,1617,
	352,1088,1618,
	353,1087,1619,
	354,1086,1620,
	355,1085,1621,
	356,1084,1622,
	357,1083,1623,
	358,1082,1624,
	359,1081,1625,
	360,1080,1626,
	361,1079,1627,
	362,1078,1628,
	363,1077,1629,
	364,1076,1630,
	365,1074,1631,
	367,1073,1632,
	368,1072,1633,
	369,1071,1634,
	370,1070,1635,
	371,1069,1636,
	372,1068,1637,
	373,1067,1638,
	374,1066,1639,
	375,1065,1640,
	376,1064,1641,
	377,1063,1642,
	378,1062,1643,
	379,1061,1644,
	380,1060,1645,
	381,1059,1646,
	382,1058,1647,
	383,1057,1648,
	384,1056,1649,
	385,1055,1650,
	386,1054,1651,
	387,1053,1652,
	388,1052,1653,
	389,1051,1654,
	390,1050,1655,
	391,1049,1656,
	392,1048,1657,
	393,1047,1658,
	394,1046,1659,
	396,1044,1660,
	397,1043,1661,
	398,1042,1662,
	400,1040,1663,
	401,1039,1664,
	402,1038,1665,
	404,1036,1666,
	405,1035,1667,
	406,1034,1668,
	408,1032,1669,
	409,1031,1670,
	410,1030,1671,
	411,1029,1672,
	413,1027,1673,
	414,1026,1674,
	415,1025,1675,
	417,1023,1676,
	418,1022,1677,
	419,1021,1678,
	421,1019,1679,
	422,1018,1680,
	423,1017,1681,
	425,1015,1682,
	426,1014,1683,
	427,1013,1684,
	428,1012,1685,
	430,1010,1686,
	431,1009,1687,
	432,1008,1688,
	434,1006,1689,
	435,1005,1690,
	436,1004,1691,
	438,1002,1692,
	439,1001,1693,
	440,1000,1694,
	442,998,1695,
	443,997,1696,
	445,995,1697,
	447,993,1698,
	448,992,1699,
	450,990,1700,
	452,988,1701,
	454,986,1702,
	455,985,1703,
	457,983,1704,
	459,981,1705,
	460,980,1706,
	462,978,1707,
	464,976,1708,
	465,975,1709,
	467,973,1710,
	469,971,1711,
	470,970,1712,
	472,968,1713,
	474,966,1714,
	476,964,1715,
	477,963,1716,
	479,961,1717,
	481,959,1718,
	482,958,1719,
	484,956,1720,
	486,954,1721,
	487,953,1722,
	489,951,1723,
	491,949,1724,
	493,947,1725,
	495,945,1726,
	497,943,1727,
	499,941,1728,
	502,938,1729,
	504,936,1730,
	506,934,1731,
	508,932,1732,
	511,929,1733,
	513,927,1734,
	515,925,1735,
	518,922,1736,
	520,920,1737,
	522,918,1738,
	524,916,1739,
	527,913,1740,
	529,911,1741,
	531,909,1742,
	533,907,1743,
	536,904,1744,
	538,902,1745,
	540,900,1746,
	543,897,1747,
	545,895,1748,
	547,893,1749,
	551,889,1750,
	554,886,1751,
	557,883,1752,
	560,880,1753,
	564,876,1754,
	567,873,1755,
	570,870,1756,
	574,866,1757,
	577,863,1758,
	580,860,1759,
	583,857,1760,
	587,853,1761,
	590,850,1762,
	593,847,1763,
	596,843,1764,
	600,840,1765,
	603,837,1766,
	609,831,1767,
	614,826,1768,
	620,820,1769,
	625,815,1770,
	631,809,1771,
	636,804,1772,
	642,798,1773,
	648,792,1774,
	653,787,1775,
	659,781,1776,
	670,770,1777,
	687,753,1778,
	704,736,1779
};

#define SQUARE_BMP_DATA_ROW	800
const uint16_t exposure_bmp_data_square[SQUARE_BMP_DATA_ROW][3]={
	320,1119,880,
	320,1119,881,
	320,1119,882,
	320,1119,883,
	320,1119,884,
	320,1119,885,
	320,1119,886,
	320,1119,887,
	320,1119,888,
	320,1119,889,
	320,1119,890,
	320,1119,891,
	320,1119,892,
	320,1119,893,
	320,1119,894,
	320,1119,895,
	320,1119,896,
	320,1119,897,
	320,1119,898,
	320,1119,899,
	320,1119,900,
	320,1119,901,
	320,1119,902,
	320,1119,903,
	320,1119,904,
	320,1119,905,
	320,1119,906,
	320,1119,907,
	320,1119,908,
	320,1119,909,
	320,1119,910,
	320,1119,911,
	320,1119,912,
	320,1119,913,
	320,1119,914,
	320,1119,915,
	320,1119,916,
	320,1119,917,
	320,1119,918,
	320,1119,919,
	320,1119,920,
	320,1119,921,
	320,1119,922,
	320,1119,923,
	320,1119,924,
	320,1119,925,
	320,1119,926,
	320,1119,927,
	320,1119,928,
	320,1119,929,
	320,1119,930,
	320,1119,931,
	320,1119,932,
	320,1119,933,
	320,1119,934,
	320,1119,935,
	320,1119,936,
	320,1119,937,
	320,1119,938,
	320,1119,939,
	320,1119,940,
	320,1119,941,
	320,1119,942,
	320,1119,943,
	320,1119,944,
	320,1119,945,
	320,1119,946,
	320,1119,947,
	320,1119,948,
	320,1119,949,
	320,1119,950,
	320,1119,951,
	320,1119,952,
	320,1119,953,
	320,1119,954,
	320,1119,955,
	320,1119,956,
	320,1119,957,
	320,1119,958,
	320,1119,959,
	320,1119,960,
	320,1119,961,
	320,1119,962,
	320,1119,963,
	320,1119,964,
	320,1119,965,
	320,1119,966,
	320,1119,967,
	320,1119,968,
	320,1119,969,
	320,1119,970,
	320,1119,971,
	320,1119,972,
	320,1119,973,
	320,1119,974,
	320,1119,975,
	320,1119,976,
	320,1119,977,
	320,1119,978,
	320,1119,979,
	320,1119,980,
	320,1119,981,
	320,1119,982,
	320,1119,983,
	320,1119,984,
	320,1119,985,
	320,1119,986,
	320,1119,987,
	320,1119,988,
	320,1119,989,
	320,1119,990,
	320,1119,991,
	320,1119,992,
	320,1119,993,
	320,1119,994,
	320,1119,995,
	320,1119,996,
	320,1119,997,
	320,1119,998,
	320,1119,999,
	320,1119,1000,
	320,1119,1001,
	320,1119,1002,
	320,1119,1003,
	320,1119,1004,
	320,1119,1005,
	320,1119,1006,
	320,1119,1007,
	320,1119,1008,
	320,1119,1009,
	320,1119,1010,
	320,1119,1011,
	320,1119,1012,
	320,1119,1013,
	320,1119,1014,
	320,1119,1015,
	320,1119,1016,
	320,1119,1017,
	320,1119,1018,
	320,1119,1019,
	320,1119,1020,
	320,1119,1021,
	320,1119,1022,
	320,1119,1023,
	320,1119,1024,
	320,1119,1025,
	320,1119,1026,
	320,1119,1027,
	320,1119,1028,
	320,1119,1029,
	320,1119,1030,
	320,1119,1031,
	320,1119,1032,
	320,1119,1033,
	320,1119,1034,
	320,1119,1035,
	320,1119,1036,
	320,1119,1037,
	320,1119,1038,
	320,1119,1039,
	320,1119,1040,
	320,1119,1041,
	320,1119,1042,
	320,1119,1043,
	320,1119,1044,
	320,1119,1045,
	320,1119,1046,
	320,1119,1047,
	320,1119,1048,
	320,1119,1049,
	320,1119,1050,
	320,1119,1051,
	320,1119,1052,
	320,1119,1053,
	320,1119,1054,
	320,1119,1055,
	320,1119,1056,
	320,1119,1057,
	320,1119,1058,
	320,1119,1059,
	320,1119,1060,
	320,1119,1061,
	320,1119,1062,
	320,1119,1063,
	320,1119,1064,
	320,1119,1065,
	320,1119,1066,
	320,1119,1067,
	320,1119,1068,
	320,1119,1069,
	320,1119,1070,
	320,1119,1071,
	320,1119,1072,
	320,1119,1073,
	320,1119,1074,
	320,1119,1075,
	320,1119,1076,
	320,1119,1077,
	320,1119,1078,
	320,1119,1079,
	320,1119,1080,
	320,1119,1081,
	320,1119,1082,
	320,1119,1083,
	320,1119,1084,
	320,1119,1085,
	320,1119,1086,
	320,1119,1087,
	320,1119,1088,
	320,1119,1089,
	320,1119,1090,
	320,1119,1091,
	320,1119,1092,
	320,1119,1093,
	320,1119,1094,
	320,1119,1095,
	320,1119,1096,
	320,1119,1097,
	320,1119,1098,
	320,1119,1099,
	320,1119,1100,
	320,1119,1101,
	320,1119,1102,
	320,1119,1103,
	320,1119,1104,
	320,1119,1105,
	320,1119,1106,
	320,1119,1107,
	320,1119,1108,
	320,1119,1109,
	320,1119,1110,
	320,1119,1111,
	320,1119,1112,
	320,1119,1113,
	320,1119,1114,
	320,1119,1115,
	320,1119,1116,
	320,1119,1117,
	320,1119,1118,
	320,1119,1119,
	320,1119,1120,
	320,1119,1121,
	320,1119,1122,
	320,1119,1123,
	320,1119,1124,
	320,1119,1125,
	320,1119,1126,
	320,1119,1127,
	320,1119,1128,
	320,1119,1129,
	320,1119,1130,
	320,1119,1131,
	320,1119,1132,
	320,1119,1133,
	320,1119,1134,
	320,1119,1135,
	320,1119,1136,
	320,1119,1137,
	320,1119,1138,
	320,1119,1139,
	320,1119,1140,
	320,1119,1141,
	320,1119,1142,
	320,1119,1143,
	320,1119,1144,
	320,1119,1145,
	320,1119,1146,
	320,1120,1147,
	320,1119,1148,
	320,1119,1149,
	320,1119,1150,
	320,1119,1151,
	320,1119,1152,
	320,1119,1153,
	320,1119,1154,
	320,1119,1155,
	320,1119,1156,
	320,1119,1157,
	320,1119,1158,
	320,1119,1159,
	320,1119,1160,
	320,1119,1161,
	320,1119,1162,
	320,1119,1163,
	320,1119,1164,
	320,1119,1165,
	320,1119,1166,
	320,1119,1167,
	320,1119,1168,
	320,1119,1169,
	320,1119,1170,
	320,1119,1171,
	320,1119,1172,
	320,1119,1173,
	320,1119,1174,
	320,1119,1175,
	320,1119,1176,
	320,1119,1177,
	320,1119,1178,
	320,1119,1179,
	320,1119,1180,
	320,1119,1181,
	320,1119,1182,
	320,1119,1183,
	320,1119,1184,
	320,1119,1185,
	320,1119,1186,
	320,1119,1187,
	320,1119,1188,
	320,1119,1189,
	320,1119,1190,
	320,1119,1191,
	320,1119,1192,
	320,1119,1193,
	320,1119,1194,
	320,1119,1195,
	320,1119,1196,
	320,1119,1197,
	320,1119,1198,
	320,1119,1199,
	320,1119,1200,
	320,1119,1201,
	320,1119,1202,
	320,1119,1203,
	320,1119,1204,
	320,1119,1205,
	320,1119,1206,
	320,1119,1207,
	320,1119,1208,
	320,1119,1209,
	320,1119,1210,
	320,1119,1211,
	320,1119,1212,
	320,1119,1213,
	320,1119,1214,
	320,1119,1215,
	320,1119,1216,
	320,1119,1217,
	320,1119,1218,
	320,1119,1219,
	320,1119,1220,
	320,1119,1221,
	320,1119,1222,
	320,1119,1223,
	320,1119,1224,
	320,1119,1225,
	320,1119,1226,
	320,1119,1227,
	320,1119,1228,
	320,1119,1229,
	320,1119,1230,
	320,1119,1231,
	320,1119,1232,
	320,1119,1233,
	320,1119,1234,
	320,1119,1235,
	320,1119,1236,
	320,1119,1237,
	320,1119,1238,
	320,1119,1239,
	320,1119,1240,
	320,1119,1241,
	320,1119,1242,
	320,1119,1243,
	320,1119,1244,
	320,1119,1245,
	320,1119,1246,
	320,1119,1247,
	320,1119,1248,
	320,1119,1249,
	320,1119,1250,
	320,1119,1251,
	320,1119,1252,
	320,1119,1253,
	320,1119,1254,
	320,1119,1255,
	320,1119,1256,
	320,1119,1257,
	320,1119,1258,
	320,1119,1259,
	320,1119,1260,
	320,1119,1261,
	320,1119,1262,
	320,1119,1263,
	320,1119,1264,
	320,1119,1265,
	320,1119,1266,
	320,1119,1267,
	320,1119,1268,
	320,1119,1269,
	320,1119,1270,
	320,1119,1271,
	320,1119,1272,
	320,1119,1273,
	320,1119,1274,
	320,1119,1275,
	320,1119,1276,
	320,1119,1277,
	320,1119,1278,
	320,1119,1279,
	320,1119,1280,
	320,1119,1281,
	320,1119,1282,
	320,1119,1283,
	320,1119,1284,
	320,1119,1285,
	320,1119,1286,
	320,1119,1287,
	320,1119,1288,
	320,1119,1289,
	320,1119,1290,
	320,1119,1291,
	320,1119,1292,
	320,1119,1293,
	320,1119,1294,
	320,1119,1295,
	320,1119,1296,
	320,1119,1297,
	320,1119,1298,
	320,1119,1299,
	320,1119,1300,
	320,1119,1301,
	320,1119,1302,
	320,1119,1303,
	320,1119,1304,
	320,1119,1305,
	320,1119,1306,
	320,1119,1307,
	320,1119,1308,
	320,1119,1309,
	320,1119,1310,
	320,1119,1311,
	320,1119,1312,
	320,1119,1313,
	320,1119,1314,
	320,1119,1315,
	320,1119,1316,
	320,1119,1317,
	320,1119,1318,
	320,1119,1319,
	320,1119,1320,
	320,1119,1321,
	320,1119,1322,
	320,1119,1323,
	320,1119,1324,
	320,1119,1325,
	320,1119,1326,
	320,1119,1327,
	320,1119,1328,
	320,1119,1329,
	320,1119,1330,
	320,1119,1331,
	320,1119,1332,
	320,1119,1333,
	320,1119,1334,
	320,1119,1335,
	320,1119,1336,
	320,1119,1337,
	320,1119,1338,
	320,1119,1339,
	320,1119,1340,
	320,1119,1341,
	320,1119,1342,
	320,1119,1343,
	320,1119,1344,
	320,1119,1345,
	320,1119,1346,
	320,1119,1347,
	320,1119,1348,
	320,1119,1349,
	320,1119,1350,
	320,1119,1351,
	320,1119,1352,
	320,1119,1353,
	320,1119,1354,
	320,1119,1355,
	320,1119,1356,
	320,1119,1357,
	320,1119,1358,
	320,1119,1359,
	320,1119,1360,
	320,1119,1361,
	320,1119,1362,
	320,1119,1363,
	320,1119,1364,
	320,1119,1365,
	320,1119,1366,
	320,1119,1367,
	320,1119,1368,
	320,1119,1369,
	320,1119,1370,
	320,1119,1371,
	320,1119,1372,
	320,1119,1373,
	320,1119,1374,
	320,1119,1375,
	320,1119,1376,
	320,1119,1377,
	320,1119,1378,
	320,1119,1379,
	320,1119,1380,
	320,1119,1381,
	320,1119,1382,
	320,1119,1383,
	320,1119,1384,
	320,1119,1385,
	320,1119,1386,
	320,1119,1387,
	320,1119,1388,
	320,1119,1389,
	320,1119,1390,
	320,1119,1391,
	320,1119,1392,
	320,1119,1393,
	320,1119,1394,
	320,1119,1395,
	320,1119,1396,
	320,1119,1397,
	320,1119,1398,
	320,1119,1399,
	320,1119,1400,
	320,1119,1401,
	320,1119,1402,
	320,1119,1403,
	320,1119,1404,
	320,1119,1405,
	320,1119,1406,
	320,1119,1407,
	320,1119,1408,
	320,1119,1409,
	320,1119,1410,
	320,1119,1411,
	320,1119,1412,
	320,1120,1413,
	320,1119,1414,
	320,1119,1415,
	320,1119,1416,
	320,1119,1417,
	320,1119,1418,
	320,1119,1419,
	320,1119,1420,
	320,1119,1421,
	320,1119,1422,
	320,1119,1423,
	320,1119,1424,
	320,1119,1425,
	320,1119,1426,
	320,1119,1427,
	320,1119,1428,
	320,1119,1429,
	320,1119,1430,
	320,1119,1431,
	320,1119,1432,
	320,1119,1433,
	320,1119,1434,
	320,1119,1435,
	320,1119,1436,
	320,1119,1437,
	320,1119,1438,
	320,1119,1439,
	320,1119,1440,
	320,1119,1441,
	320,1119,1442,
	320,1119,1443,
	320,1119,1444,
	320,1119,1445,
	320,1119,1446,
	320,1119,1447,
	320,1119,1448,
	320,1119,1449,
	320,1119,1450,
	320,1119,1451,
	320,1119,1452,
	320,1119,1453,
	320,1119,1454,
	320,1119,1455,
	320,1119,1456,
	320,1119,1457,
	320,1119,1458,
	320,1119,1459,
	320,1119,1460,
	320,1119,1461,
	320,1119,1462,
	320,1119,1463,
	320,1119,1464,
	320,1119,1465,
	320,1119,1466,
	320,1119,1467,
	320,1119,1468,
	320,1119,1469,
	320,1119,1470,
	320,1119,1471,
	320,1119,1472,
	320,1119,1473,
	320,1119,1474,
	320,1119,1475,
	320,1119,1476,
	320,1119,1477,
	320,1119,1478,
	320,1119,1479,
	320,1119,1480,
	320,1119,1481,
	320,1119,1482,
	320,1119,1483,
	320,1119,1484,
	320,1119,1485,
	320,1119,1486,
	320,1119,1487,
	320,1119,1488,
	320,1119,1489,
	320,1119,1490,
	320,1119,1491,
	320,1119,1492,
	320,1119,1493,
	320,1119,1494,
	320,1119,1495,
	320,1119,1496,
	320,1119,1497,
	320,1119,1498,
	320,1119,1499,
	320,1119,1500,
	320,1119,1501,
	320,1119,1502,
	320,1119,1503,
	320,1119,1504,
	320,1119,1505,
	320,1119,1506,
	320,1119,1507,
	320,1119,1508,
	320,1119,1509,
	320,1119,1510,
	320,1119,1511,
	320,1119,1512,
	320,1119,1513,
	320,1119,1514,
	320,1119,1515,
	320,1119,1516,
	320,1119,1517,
	320,1119,1518,
	320,1119,1519,
	320,1119,1520,
	320,1119,1521,
	320,1119,1522,
	320,1119,1523,
	320,1119,1524,
	320,1119,1525,
	320,1119,1526,
	320,1119,1527,
	320,1119,1528,
	320,1119,1529,
	320,1119,1530,
	320,1119,1531,
	320,1119,1532,
	320,1119,1533,
	320,1119,1534,
	320,1119,1535,
	320,1119,1536,
	320,1119,1537,
	320,1119,1538,
	320,1119,1539,
	320,1119,1540,
	320,1119,1541,
	320,1119,1542,
	320,1119,1543,
	320,1119,1544,
	320,1119,1545,
	320,1119,1546,
	320,1119,1547,
	320,1119,1548,
	320,1119,1549,
	320,1119,1550,
	320,1119,1551,
	320,1119,1552,
	320,1119,1553,
	320,1119,1554,
	320,1119,1555,
	320,1119,1556,
	320,1119,1557,
	320,1119,1558,
	320,1119,1559,
	320,1119,1560,
	320,1119,1561,
	320,1119,1562,
	320,1119,1563,
	320,1119,1564,
	320,1119,1565,
	320,1119,1566,
	320,1119,1567,
	320,1119,1568,
	320,1119,1569,
	320,1119,1570,
	320,1119,1571,
	320,1119,1572,
	320,1119,1573,
	320,1119,1574,
	320,1119,1575,
	320,1119,1576,
	320,1119,1577,
	320,1119,1578,
	320,1119,1579,
	320,1119,1580,
	320,1119,1581,
	320,1119,1582,
	320,1119,1583,
	320,1119,1584,
	320,1119,1585,
	320,1119,1586,
	320,1119,1587,
	320,1119,1588,
	320,1119,1589,
	320,1119,1590,
	320,1119,1591,
	320,1119,1592,
	320,1119,1593,
	320,1119,1594,
	320,1119,1595,
	320,1119,1596,
	320,1119,1597,
	320,1119,1598,
	320,1119,1599,
	320,1119,1600,
	320,1119,1601,
	320,1119,1602,
	320,1119,1603,
	320,1119,1604,
	320,1119,1605,
	320,1119,1606,
	320,1119,1607,
	320,1119,1608,
	320,1119,1609,
	320,1119,1610,
	320,1119,1611,
	320,1119,1612,
	320,1119,1613,
	320,1119,1614,
	320,1119,1615,
	320,1119,1616,
	320,1119,1617,
	320,1119,1618,
	320,1119,1619,
	320,1119,1620,
	320,1119,1621,
	320,1119,1622,
	320,1119,1623,
	320,1119,1624,
	320,1119,1625,
	320,1119,1626,
	320,1119,1627,
	320,1119,1628,
	320,1119,1629,
	320,1119,1630,
	320,1119,1631,
	320,1119,1632,
	320,1119,1633,
	320,1119,1634,
	320,1119,1635,
	320,1119,1636,
	320,1119,1637,
	320,1119,1638,
	320,1119,1639,
	320,1119,1640,
	320,1119,1641,
	320,1119,1642,
	320,1119,1643,
	320,1119,1644,
	320,1119,1645,
	320,1119,1646,
	320,1119,1647,
	320,1119,1648,
	320,1119,1649,
	320,1119,1650,
	320,1119,1651,
	320,1119,1652,
	320,1119,1653,
	320,1119,1654,
	320,1119,1655,
	320,1119,1656,
	320,1119,1657,
	320,1119,1658,
	320,1119,1659,
	320,1119,1660,
	320,1119,1661,
	320,1119,1662,
	320,1119,1663,
	320,1119,1664,
	320,1119,1665,
	320,1119,1666,
	320,1119,1667,
	320,1119,1668,
	320,1119,1669,
	320,1119,1670,
	320,1119,1671,
	320,1119,1672,
	320,1119,1673,
	320,1119,1674,
	320,1119,1675,
	320,1119,1676,
	320,1119,1677,
	320,1119,1678,
	320,1119,1679,
};








void		UVD_Init();
void		UVD_Sleep();
void		UVD_Wakeup();
void		UVD_ExposSetCircle();
void		UVD_ExposSetSquare();




#endif // _uvdisplay_H