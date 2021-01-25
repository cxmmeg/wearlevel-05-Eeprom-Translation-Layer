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




**++++++++++++++++++++4.6+++++++++++++++**

负载自适应测试: 

T1,对同一个逻辑地址进行10,000次写入操作，每次写入长度20B的数据:

 { 224, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 220, 222, 220, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 222, 224, 224, 222, 192, 222, 192 }
write speed            192 B/sec
spend time             1038 sec    =>  1120 sec (without pagetable cache)
overhead ratio         1.060300
standard deviation     144.180440
RAM cost               520 B
dualcache hit rate     0.999950        
total write cycles : 20000 

T2,随机地址写入

page erase cnt :{ 316, 267, 303, 285, 316, 333, 301, 295, 290, 305, 289, 272, 295, 302, 280, 288, 306, 312, 295, 288, 300, 287, 286, 287, 301, 301, 294, 308, 306, 288, 315, 299, 270, 281, 295, 279, 291, 288, 277, 289, 281, 259, 308, 284, 295, 285, 309, 299, 300, 277, 299, 294, 281, 307, 303, 303, 301, 318, 298, 294, 294, 307, 288, 294, 306, 310, 280, 309, 289, 310, 301, 288, 279, 296, 294, 289, 288, 306, 295, 289, 329, 294, 290, 303, 302, 299, 297, 301, 286, 287, 315, 307, 301, 288, 312, 296, 286, 281, 318, 287 }
write speed            204 B/sec
spend time             1086 sec    =>  1099 sec (without pagetable cache)
overhead ratio         1.035804
standard deviation     126.261630
RAM cost               520 B
dualcache hit rate     0.969135        
total write cycles : 28544 

缓存映射表测试: 

| 测试用例 | 请求数 | 页面总数 | 访问模式          |
| -------- | ------ | -------- | ----------------- |
| T1       | 10,000 | 1,000    | 随机访问          |
| T2       | 10,000 | 1,000    | 随机访问+顺序扫描 |

性能指标:  命中率  运行时间

**T1,2Q**
hitrate time    cachesize
0.33    53.00   40
0.45    47.00   80
0.58    39.00   120
0.66    34.00   160 

**T1,LRU**
0.37    51.00   40
0.52    42.00   80
0.62    37.00   120
0.69    32.00   160

**T1,no cache**
-       68      - 

**T2,no cache**
-       68      - 

**T2,2Q**
0.25    58.00   40
0.36    52.00   80
0.46    48.00   120
0.51    43.00   160

**T2,LRU**
0.23    60.00   40
0.31    57.00   80
0.36    54.00   120
0.43    50.00   160




**--------------------4.6---------------**


**+++++++++++++++PWL与DP初步对比+++++++++++++**
TestCase: MultiWriteSimulateTest(20000)

NOWL:
[INFO performance.cc:65] write speed            335 B/sec
[INFO performance.cc:66] spend time             896 sec
[INFO performance.cc:67] overhead ratio         1.000000
[INFO performance.cc:68] standard deviation     21610.196000
[INFO performance.cc:69] RAM cost               900 B
[INFO performance.cc:70] dualcache hit rate     0.999900        
[INFO performance.cc:71] total write cycles : 30033 
[INFO performance.cc:72] extra write cycles : 0 
{ 0, 113, 0, 0, 0, 0, 0, 0, 0, 0, 18700, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 11220, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0 }


PDP:
[INFO performance.cc:65] write speed            327 B/sec
[INFO performance.cc:66] spend time             918 sec
[INFO performance.cc:67] overhead ratio         1.009922
[INFO performance.cc:68] standard deviation     492.037600
[INFO performance.cc:69] RAM cost               420 B
[INFO performance.cc:70] dualcache hit rate     0.999900        
[INFO performance.cc:71] total write cycles : 30033 
[INFO performance.cc:72] extra write cycles : 298 
page ec:{ 377, 
387, 381, 249, 375, 246, 368, 247, 370, 245, 251, 243, 365, 243, 359, 241, 359, 240, 351, 239, 353, 
237, 347, 238, 341, 236, 343, 234, 337, 234, 333, 231, 333, 232, 325, 231, 327, 228, 323, 229, 317, 
227, 317, 225, 313, 225, 309, 224, 309, 223, 249, 222, 301, 220, 303, 220, 299, 219, 293, 249, 295, 
217, 291, 327, 287, 227, 285, 326, 279, 324, 281, 325, 277, 325, 273, 324, 273, 324, 269, 321, 267, 
322, 265, 316, 259, 323, 261, 317, 257, 322, 253, 319, 253, 319, 249, 318, 247, 319, 245, 317, 324, 
387, 243, 310, 325}

DP: TH=60
[INFO performance.cc:54] write speed            290 B/sec
[INFO performance.cc:55] overhead ratio         1.030866
[INFO performance.cc:56] standard deviation     266.634580
[INFO performance.cc:57] RAM cost               1260 B
[INFO performance.cc:58] total write cycles : 30033 
[INFO performance.cc:59] extra write cycles : 927 
page ec:{ 310, 
309, 300, 311, 310, 249, 249, 311, 310, 249, 309, 249, 310, 249, 310, 249, 310, 249, 310, 249, 310, 
249, 309, 249, 310, 249, 309, 249, 310, 249, 310, 249, 309, 249, 310, 249, 310, 249, 309, 249, 310, 
249, 309, 249, 309, 249, 310, 249, 309, 249, 249, 250, 310, 292, 310, 311, 309, 311, 310, 311, 309, 
311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 
311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 
311, 310, 311, 310, }

**---------------PWL与DP初步对比-------------**



**++++++++++++++PDP与DP详细对比++++++++++++++**
PDP:
endurace = 500

cycles : [ 552, 1034, 1516, 1998, 2490, 2982, 3474, 4015, 4497, 4979, 5461, 5953, 6495, 6976, 7458, 7940, 8432, 8924, 9476, 9958, 10439, 10921, 11403, 11955, 12437, 12919, 13400, 13882, 14374, 14876, 15418, 15900, 16382, 16863, 17355, 17897, 18379, 18861, 19343, 19835, 20326, 20818, 21360, 21842, 22324, 22806, 23298, 23839, 24321, 24803, 25285, 25767, 26269, 26761, 27302, 27784, 28266, 28748, 29230, 29711, 30203, 30745, 31227, 31709, 32191, 32672, 33154, 33636, 34118, 34610, 35102, 35594, 36075, 36617, 37099, 37581, 38063, 38544, 39026, 39508, 39990, 40472, 40953, 41435, 41917, 42399, 42881, 43362, 43844, 44326, 44748, 45219, 45701, 46173, 46645, 47126, 47608, 48020, 48502, 48923  ]

overhead ratio : [ 1.0036, 1.0058, 1.0066, 1.0080, 1.0072, 1.0074, 1.0075, 1.0080, 1.0080, 1.0080, 1.0081, 1.0081, 1.0083, 1.0083, 1.0086, 1.0083, 1.0083, 1.0085, 1.0087, 1.0086, 1.0088, 1.0088, 1.0089, 1.0087, 1.0088, 1.0087, 1.0087, 1.0086, 1.0088, 1.0087, 1.0088, 1.0087, 1.0086, 1.0085, 1.0086, 1.0085, 1.0087, 1.0087, 1.0087, 1.0088, 1.0087, 1.0088, 1.0088, 1.0088, 1.0089, 1.0088, 1.0089, 1.0091, 1.0091, 1.0091, 1.0091, 1.0092, 1.0091, 1.0093, 1.0093, 1.0093, 1.0094, 1.0095, 1.0096, 1.0098, 1.0099, 1.0101, 1.0102, 1.0103, 1.0106, 1.0107, 1.0109, 1.0110, 1.0112, 1.0113, 1.0115, 1.0117, 1.0118, 1.0120, 1.0122, 1.0124, 1.0127, 1.0131, 1.0132, 1.0135, 1.0137, 1.0140, 1.0142, 1.0144, 1.0147, 1.0152, 1.0155, 1.0158, 1.0162, 1.0165, 1.0169, 1.0173, 1.0177, 1.0181, 1.0188, 1.0193, 1.0199, 1.0205, 1.0213, 1.0222  ]

standard deviation : [ 332.10, 467.13, 578.67, 669.91, 729.12, 786.38, 843.92, 891.70, 936.66, 974.03, 997.55, 1025.95, 1052.94, 1075.32, 1098.13, 1108.33, 1122.93, 1128.67, 1139.29, 1139.39, 1144.81, 1140.41, 1140.66, 1150.95, 1194.33, 1224.23, 1248.95, 1257.44, 1283.58, 1296.16, 1313.76, 1325.97, 1338.25, 1338.64, 1335.84, 1342.83, 1330.90, 1332.64, 1316.06, 1307.51, 1294.53, 1274.09, 1259.16, 1232.98, 1206.97, 1185.92, 1147.07, 1103.57, 1066.49, 1011.77, 957.88, 905.29, 838.94, 752.36, 638.94, 551.57, 541.02, 530.97, 515.87, 503.05, 484.80, 462.06, 441.99, 420.71, 393.68, 365.20, 332.48, 294.79, 255.39, 220.16, 203.72, 208.33, 233.89, 252.62, 264.50, 271.76, 269.02, 265.38, 252.52, 235.82, 212.58, 177.52, 143.35, 120.69, 121.14, 144.82, 167.96, 187.85, 191.02, 181.72, 165.14, 125.57, 82.37, 69.35, 103.61, 121.11, 115.55, 93.08, 37.05, 65.00  ]
**--------------PDP与DP详细对比--------------**
