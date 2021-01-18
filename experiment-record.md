# using BR-Tree time (TestHotPageToColdPage(1500)) : 4 mins

# bitmap without pagetable's cache TestHotPageToColdPage(1500) takes time: 9 mins

start time : 
20/12/2 15:33:56 

end time : 
20/12/2 15:43:6 

**next step:using heap-cache**

# bitmap with pagetable's cache TestHotPageToColdPage(1500) takes time: 10 mins

start time : 
20/12/4 15:43:30 

end time : 
20/12/4 15:53:6 

# bit map with pagetable's cache , dualpool border cache , TestHotPageToColdPage(1500) takes time: 6 mins

start time : 
20/12/8 15:42:28 

end time : 
20/12/8 15:48:42 

**TestHotPageToColdPage(1500) -> single page**

takes 3mins

[INFO main.cc:66] start start_time : 
[INFO rtc.cc:1501] 20/12/9 15:45:27 

[INFO main.cc:88] end time : 
[INFO rtc.cc:1501] 20/12/9 15:48:27 


**rm 5 borders**

takes 3mins

[INFO main.cc:66] start start_time : 
[INFO rtc.cc:1501] 20/12/9 20:49:37 

[INFO main.cc:88] end time : 
[INFO rtc.cc:1501] 20/12/9 20:52:33 


**++++++++++++SampleSimulation(200)++++++++++++++**


[INFO performance.cc:51] ++++++++++++ performance +++++++++++++++

[INFO performance.cc:52] write speed		105 B/sec
[INFO performance.cc:53] overhead ratio		1.064643
[INFO performance.cc:54] standard deviation	153.707520
[INFO performance.cc:55] RAM cost		572 B
[INFO performance.cc:56] total write cycles : 64490 
[INFO performance.cc:57] ------------ performance ---------------

++++++++++++++++Erase Cycle Pool Info+++++++++++++

hot pool : 
ppn : 13 	 erase cycle : 81 	 effective erase cycle : 15 
ppn : 15 	 erase cycle : 80 	 effective erase cycle : 14 
ppn : 21 	 erase cycle : 84 	 effective erase cycle : 18 
ppn : 23 	 erase cycle : 81 	 effective erase cycle : 15 
cold pool : 
ppn : 0 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 1 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 2 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 3 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 4 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 5 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 6 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 7 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 8 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 9 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 10 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 11 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 12 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 14 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 16 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 17 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 18 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 19 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 20 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 22 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 24 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 25 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 26 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 27 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 28 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 29 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 30 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 31 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 32 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 33 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 34 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 35 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 36 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 37 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 38 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 39 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 40 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 41 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 42 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 43 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 44 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 45 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 46 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 47 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 48 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 49 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 50 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 51 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 52 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 53 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 54 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 55 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 56 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 57 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 58 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 59 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 60 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 61 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 62 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 63 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 64 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 65 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 66 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 67 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 68 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 69 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 70 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 71 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 72 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 73 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 74 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 75 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 76 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 77 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 78 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 79 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 80 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 81 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 82 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 83 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 84 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 85 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 86 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 87 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 88 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 89 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 90 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 91 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 92 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 93 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 94 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 95 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 96 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 97 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 98 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 99 	 erase cycle : 66 	 effective erase cycle : 0 
ppn : 100 	 erase cycle : 85 	 effective erase cycle : 4 
ppn : 101 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 102 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 103 	 erase cycle : 67 	 effective erase cycle : 0 
ppn : 104 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 105 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 106 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 107 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 108 	 erase cycle : 96 	 effective erase cycle : 0 
ppn : 109 	 erase cycle : 96 	 effective erase cycle : 0 
ppn : 110 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 111 	 erase cycle : 96 	 effective erase cycle : 0 
ppn : 112 	 erase cycle : 65 	 effective erase cycle : 0 
---------------Erase Cycle Pool Info---------------

thresh_hold : 30 ,hotpool size : 4 , coldpool size : 109 


**----------------SampleSimulation(200)------------------**


**++++++++++++++++++++++++MultiWriteTest(120)+++++++++++++++++++++++++**

const unsigned long long ROM_SIZE	 = 2KB
const unsigned char	 LOGIC_PAGE_SIZE = 10;
const unsigned int	 THRESH_HOLD	 = 30;

[INFO performance.cc:51] ++++++++++++ performance +++++++++++++++
[INFO performance.cc:52] write speed        110 B/sec
[INFO performance.cc:53] overhead ratio     1.065034
[INFO performance.cc:54] standard deviation 99.433400
[INFO performance.cc:55] RAM cost       584 B
[INFO etl.cc:443] dualpool ram cost : 440 
[INFO etl.cc:445] pagetable ram cost : 132 
[INFO performance.cc:56] total write cycles : 46896 
[INFO performance.cc:57] ------------ performance ---------------

++++++++++++++++Erase Cycle Pool Info+++++++++++++

hot pool : 
ppn : 77     erase cycle : 61    effective erase cycle : 27 
ppn : 79     erase cycle : 60    effective erase cycle : 26 
ppn : 85     erase cycle : 51    effective erase cycle : 17 
ppn : 87     erase cycle : 49    effective erase cycle : 15 
cold pool : 
ppn : 0      erase cycle : 63    effective erase cycle : 0 
ppn : 1      erase cycle : 64    effective erase cycle : 0 
ppn : 2      erase cycle : 64    effective erase cycle : 0 
ppn : 3      erase cycle : 65    effective erase cycle : 0 
ppn : 4      erase cycle : 63    effective erase cycle : 0 
ppn : 5      erase cycle : 34    effective erase cycle : 0 
ppn : 6      erase cycle : 63    effective erase cycle : 0 
ppn : 7      erase cycle : 65    effective erase cycle : 0 
ppn : 8      erase cycle : 63    effective erase cycle : 0 
ppn : 9      erase cycle : 65    effective erase cycle : 0 
ppn : 10     erase cycle : 65    effective erase cycle : 0 
ppn : 11     erase cycle : 35    effective erase cycle : 0 
ppn : 12     erase cycle : 63    effective erase cycle : 0 
ppn : 13     erase cycle : 65    effective erase cycle : 0 
ppn : 14     erase cycle : 63    effective erase cycle : 0 
ppn : 15     erase cycle : 65    effective erase cycle : 0 
ppn : 16     erase cycle : 63    effective erase cycle : 0 
ppn : 17     erase cycle : 65    effective erase cycle : 0 
ppn : 18     erase cycle : 63    effective erase cycle : 0 
ppn : 19     erase cycle : 65    effective erase cycle : 0 
ppn : 20     erase cycle : 63    effective erase cycle : 0 
ppn : 21     erase cycle : 65    effective erase cycle : 0 
ppn : 22     erase cycle : 63    effective erase cycle : 0 
ppn : 23     erase cycle : 65    effective erase cycle : 0 
ppn : 24     erase cycle : 63    effective erase cycle : 0 
ppn : 25     erase cycle : 65    effective erase cycle : 0 
ppn : 26     erase cycle : 63    effective erase cycle : 0 
ppn : 27     erase cycle : 65    effective erase cycle : 0 
ppn : 28     erase cycle : 63    effective erase cycle : 0 
ppn : 29     erase cycle : 65    effective erase cycle : 0 
ppn : 30     erase cycle : 63    effective erase cycle : 0 
ppn : 31     erase cycle : 65    effective erase cycle : 0 
ppn : 32     erase cycle : 63    effective erase cycle : 0 
ppn : 33     erase cycle : 65    effective erase cycle : 0 
ppn : 34     erase cycle : 63    effective erase cycle : 0 
ppn : 35     erase cycle : 65    effective erase cycle : 0 
ppn : 36     erase cycle : 63    effective erase cycle : 0 
ppn : 37     erase cycle : 65    effective erase cycle : 0 
ppn : 38     erase cycle : 63    effective erase cycle : 0 
ppn : 39     erase cycle : 65    effective erase cycle : 0 
ppn : 40     erase cycle : 63    effective erase cycle : 0 
ppn : 41     erase cycle : 65    effective erase cycle : 0 
ppn : 42     erase cycle : 63    effective erase cycle : 0 
ppn : 43     erase cycle : 65    effective erase cycle : 0 
ppn : 44     erase cycle : 63    effective erase cycle : 0 
ppn : 45     erase cycle : 65    effective erase cycle : 0 
ppn : 46     erase cycle : 63    effective erase cycle : 0 
ppn : 47     erase cycle : 65    effective erase cycle : 0 
ppn : 48     erase cycle : 63    effective erase cycle : 0 
ppn : 49     erase cycle : 65    effective erase cycle : 0 
ppn : 50     erase cycle : 63    effective erase cycle : 0 
ppn : 51     erase cycle : 65    effective erase cycle : 0 
ppn : 52     erase cycle : 63    effective erase cycle : 0 
ppn : 53     erase cycle : 65    effective erase cycle : 0 
ppn : 54     erase cycle : 63    effective erase cycle : 0 
ppn : 55     erase cycle : 65    effective erase cycle : 0 
ppn : 56     erase cycle : 63    effective erase cycle : 0 
ppn : 57     erase cycle : 65    effective erase cycle : 0 
ppn : 58     erase cycle : 63    effective erase cycle : 0 
ppn : 59     erase cycle : 65    effective erase cycle : 0 
ppn : 60     erase cycle : 63    effective erase cycle : 0 
ppn : 61     erase cycle : 65    effective erase cycle : 0 
ppn : 62     erase cycle : 63    effective erase cycle : 0 
ppn : 63     erase cycle : 65    effective erase cycle : 0 
ppn : 64     erase cycle : 63    effective erase cycle : 0 
ppn : 65     erase cycle : 65    effective erase cycle : 0 
ppn : 66     erase cycle : 63    effective erase cycle : 0 
ppn : 67     erase cycle : 65    effective erase cycle : 0 
ppn : 68     erase cycle : 63    effective erase cycle : 0 
ppn : 69     erase cycle : 65    effective erase cycle : 0 
ppn : 70     erase cycle : 63    effective erase cycle : 0 
ppn : 71     erase cycle : 65    effective erase cycle : 0 
ppn : 72     erase cycle : 63    effective erase cycle : 0 
ppn : 73     erase cycle : 65    effective erase cycle : 0 
ppn : 74     erase cycle : 63    effective erase cycle : 0 
ppn : 75     erase cycle : 65    effective erase cycle : 0 
ppn : 76     erase cycle : 63    effective erase cycle : 0 
ppn : 78     erase cycle : 63    effective erase cycle : 0 
ppn : 80     erase cycle : 63    effective erase cycle : 0 
ppn : 81     erase cycle : 65    effective erase cycle : 0 
ppn : 82     erase cycle : 63    effective erase cycle : 0 
ppn : 83     erase cycle : 65    effective erase cycle : 0 
ppn : 84     erase cycle : 63    effective erase cycle : 0 
ppn : 86     erase cycle : 63    effective erase cycle : 0 
ppn : 88     erase cycle : 63    effective erase cycle : 0 
ppn : 89     erase cycle : 33    effective erase cycle : 0 
ppn : 90     erase cycle : 63    effective erase cycle : 0 
ppn : 91     erase cycle : 33    effective erase cycle : 0 
ppn : 92     erase cycle : 63    effective erase cycle : 0 
ppn : 93     erase cycle : 33    effective erase cycle : 0 
ppn : 94     erase cycle : 63    effective erase cycle : 0 
ppn : 95     erase cycle : 33    effective erase cycle : 0 
ppn : 96     erase cycle : 63    effective erase cycle : 0 
ppn : 97     erase cycle : 33    effective erase cycle : 0 
ppn : 98     erase cycle : 63    effective erase cycle : 0 
ppn : 99     erase cycle : 33    effective erase cycle : 0 
ppn : 100    erase cycle : 33    effective erase cycle : 0 
ppn : 101    erase cycle : 33    effective erase cycle : 0 
ppn : 102    erase cycle : 63    effective erase cycle : 0 
ppn : 103    erase cycle : 33    effective erase cycle : 0 
ppn : 104    erase cycle : 65    effective erase cycle : 0 
ppn : 105    erase cycle : 44    effective erase cycle : 8 
ppn : 106    erase cycle : 65    effective erase cycle : 0 
ppn : 107    erase cycle : 63    effective erase cycle : 0 
ppn : 108    erase cycle : 63    effective erase cycle : 0 
ppn : 109    erase cycle : 64    effective erase cycle : 0 
ppn : 110    erase cycle : 65    effective erase cycle : 0 
ppn : 111    erase cycle : 63    effective erase cycle : 0 
ppn : 112    erase cycle : 65    effective erase cycle : 0 
---------------Erase Cycle Pool Info---------------

thresh_hold : 30 ,hotpool size : 4 , coldpool size : 109 

[ 63,  63,  64,  62,  33,  62,  64,  62,  64,  64 ],
[ 34,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  60,  62,  59,  62 ],
[ 64,  62,  64,  62,  50,  62,  48,  62,  32,  62 ],
[ 32,  62,  32,  62,  32,  62,  32,  62,  32,  32 ],
[ 32,  62,  32,  64,  43,  64,  62,  62,  63,  64 ],


**----------------------MultiWriteTest(120)-------------------------**


**+++++++++++++++++relation btw write cycles and performance+++++++++++++++++**

RAM: 863(dualpool) + 1020B(pagetable)

ROM_SIZE	 = 16KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 5;
write speed = 22B/sec

cycles : [ 5010, 10079, 15069, 20018, 25007, 30017, 35086, 40075, 45025, 50014, 55003, 60093, 65082, 70031, 75021, 80010, 85100, 90089, 95038, 100028, 105017, 110106, 115096, 120045, 125034, 130004, 135113, 140103, 145052, 150041, 155011, 160000, 165109, 170059, 175048, 180017, 185007, 190116, 195065, 200055 ]

overhead ratio : [ 1.2898, 1.3076, 1.3372, 1.3520, 1.3610, 1.3670, 1.3714, 1.3745, 1.3771, 1.3790, 1.3806, 1.3820, 1.3831, 1.3841, 1.3850, 1.3857, 1.3864, 1.3870, 1.3875, 1.3880, 1.3884, 1.3888, 1.3891, 1.3895, 1.3898, 1.3900, 1.3903, 1.3905, 1.3908, 1.3910, 1.3911, 1.3913, 1.3915, 1.3917, 1.3918, 1.3919, 1.3921, 1.3922, 1.3924, 1.3925  ]

standard deviation : [ 125.47, 76.54, 80.54, 83.76, 76.93, 80.49, 85.02, 75.99, 62.81, 46.03, 57.57, 74.28, 79.67, 83.06, 77.78, 84.66, 83.84, 76.84, 65.64, 49.84, 52.07, 71.54, 83.32, 82.29, 78.67, 81.71, 83.06, 77.54, 73.90, 53.89, 43.84, 70.51, 81.00, 81.60, 79.46, 79.20, 81.12, 78.34, 74.63, 56.75  ]
thresh_hold : 5 ,hotpool size : 5 , coldpool size : 904 

ROM_SIZE	 = 16KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 30;
write speed = 66B/sec

cycles : [ 5010, 10079, 15069, 20018, 25007, 30017, 35086, 40075, 45025, 50014, 55003, 60093, 65082, 70031, 75021, 80010, 85100, 90089, 95038, 100028, 105017, 110106, 115096, 120045, 125034, 130004, 135113, 140103, 145052, 150041, 155011, 160000, 165109, 170059, 175048, 180017, 185007, 190116, 195065, 200055 ]

overhead ratio : [ 1.0659, 1.0661, 1.0630, 1.0557, 1.0512, 1.0483, 1.0461, 1.0445, 1.0463, 1.0482, 1.0498, 1.0512, 1.0524, 1.0534, 1.0543, 1.0551, 1.0556, 1.0562, 1.0568, 1.0572, 1.0577, 1.0581, 1.0584, 1.0588, 1.0591, 1.0594, 1.0596, 1.0598, 1.0600, 1.0603, 1.0605, 1.0606, 1.0608, 1.0610, 1.0611, 1.0613, 1.0614, 1.0616, 1.0616, 1.0618]

standard deviation : [372.99, 465.47, 525.04, 619.47, 660.74, 654.74, 600.15, 483.61, 439.92, 348.80, 118.30, 372.71, 445.54, 451.59, 400.79, 235.19, 302.54, 422.92, 451.81, 432.47, 331.34, 176.40, 382.99, 448.13, 449.28, 391.07, 196.25, 317.89, 427.80, 450.68, 428.34, 317.89, 209.41, 391.71, 448.95, 446.91, 381.95, 159.17, 334.19, 433.33  ]


ROM_SIZE	 = 16KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
write_speed : 115B/sec

cycles : [ 5010, 10079, 15069, 20018, 25007, 30017, 35086, 40075, 45025, 50014, 55003, 60093, 65082, 70031, 75021, 80010, 85100, 90089, 95038, 100028, 105017, 110106, 115096, 120045, 125034, 130004, 135113, 140103, 145052, 150041, 155011, 160000, 165109, 170059, 175048, 180017, 185007, 190116, 195065, 200055,  ]

overhead ratio : [ 1.0196, 1.0196, 1.0198, 1.0199, 1.0198, 1.0199, 1.0199, 1.0198, 1.0199, 1.0190, 1.0181, 1.0175, 1.0169, 1.0164, 1.0160, 1.0156, 1.0153, 1.0150, 1.0147, 1.0145, 1.0143, 1.0141, 1.0139, 1.0137, 1.0136, 1.0135, 1.0133, 1.0134, 1.0137, 1.0139, 1.0141, 1.0143, 1.0144, 1.0146, 1.0147, 1.0149, 1.0150, 1.0151, 1.0152, 1.0154,  ]

standard deviation : [ 694.01, 960.29, 1141.41, 1269.57, 1370.92, 1442.31, 1493.02, 1526.23, 1534.51, 1676.31, 1799.20, 1900.13, 1982.71, 2045.93, 2095.23, 2129.29, 2151.14, 2159.99, 2154.51, 2138.01, 2104.18, 2058.13, 1996.47, 1918.17, 1824.25, 1704.54, 1556.41, 1509.89, 1487.47, 1448.74, 1386.57, 1300.24, 1182.55, 1023.71, 804.06, 432.83, 552.81, 871.84, 1069.88, 1213.61,  ]

thresh_hold : 100 ,hotpool size : 4 , coldpool size : 905

**-----------------relation btw write cycles and performance-----------------**


**+++++++++++++++++dual cache : main_cache ratio with hit ratio++++++++++++++++++**
ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
total write cycles : 10079 
dualcache hit rate 0.914700  
main cache rate : 0.7


ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
total write cycles : 10079 
main cache rate : 0.6
dualcache hit rate 0.914700 

ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
total write cycles : 10079 
main cache rate : 0.5
dualcache hit rate 0.914700 
 
ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
total write cycles : 10079 
main cache rate : 0.1
dualcache hit rate 0.914700 
**----------------dual cache : main_cache ratio with hit ratio---------------------**


**+++++++++++++++++dual cache : dualcache size with hit ratio++++++++++++++++++**

ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
cycle = 10000;

cache capacity ratio : 0.03
dualcache hit rate     0.667565
pagetable ram cost : 36B 

cache capacity ratio : 0.05
dualcache hit rate     0.667565
pagetable ram cost : 60B 

cache capacity ratio : 0.1
dualcache hit rate     0.667565
pagetable ram cost : 123B 

cache capacity ratio : 0.15
dualcache hit rate    0.66  
pagetable ram cost : 192B 

cache capacity ratio : 0.2
dualcache hit rate    0.980392  
pagetable ram cost : 264B 

 

**----------------dual cache : dualcache size with hit ratio---------------------**