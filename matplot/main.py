import makefigure as mf
import data


def main():

    #     mf.makeBarEdgecachedpVsOrigindp(
    #         [50.55, 1.05, 100, 312], [57.63, 1.06, 138, 1356], 10000, True)

    # ['standard deviation', 'overhead ratio', 'write speed', 'ram cost']

    #     hotdata = [
    #         [96,  96,  96,  94,  94,  94,  96,  94,  96,  96],
    #         [94,  94,  96,  94,  96,  94,  96,  94,  96,  94],
    #         [96,  94,  96,  94,  96,  94,  96,  94,  96,  94],
    #         [96,  94,  96,  94,  96,  94,  96,  94,  96,  94],
    #         [96,  94,  96,  94,  96,  94,  96,  94,  96,  94],
    #         [96,  94,  96,  94,  96,  94,  96,  94,  96,  94],
    #         [96,  94,  96,  94,  96,  94,  96,  94,  96,  94],
    #         [96,  94,  96,  94,  96,  94,  96,  94,  96,  94],
    #         [96,  94,  96,  94,  96,  94,  96,  94,  96,  94],
    #         [96,  94,  96,  94,  96,  94,  96,  94,  65,  66],
    #         [94,  95,  82,  80,  94,  79,  96,  95,  96,  90],
    #     ]

    #     mf.drawHot(10, 11, hotdata, 10000, 'edgecache-hotfigure', True)

    #     cycle_data = [5010, 10079, 15069, 20018, 25007, 30017, 35086, 40075, 45025, 50014, 55003, 60093, 65082, 70031, 75021, 80010, 85100, 90089, 95038, 100028,
    #                   105017, 110106, 115096, 120045, 125034, 130004, 135113, 140103, 145052, 150041, 155011, 160000, 165109, 170059, 175048, 180017, 185007, 190116, 195065, 200055]
    #     overhead_ratio = [1.2898, 1.3076, 1.3372, 1.3520, 1.3610, 1.3670, 1.3714, 1.3745, 1.3771, 1.3790, 1.3806, 1.3820, 1.3831, 1.3841, 1.3850, 1.3857, 1.3864, 1.3870, 1.3875,
    #                       1.3880, 1.3884, 1.3888, 1.3891, 1.3895, 1.3898, 1.3900, 1.3903, 1.3905, 1.3908, 1.3910, 1.3911, 1.3913, 1.3915, 1.3917, 1.3918, 1.3919, 1.3921, 1.3922, 1.3924, 1.3925]

    #     standard_deviation = [125.47, 76.54, 80.54, 83.76, 76.93, 80.49, 85.02, 75.99, 62.81, 46.03, 57.57, 74.28, 79.67, 83.06, 77.78, 84.66, 83.84, 76.84,
    #                           65.64, 49.84, 52.07, 71.54, 83.32, 82.29, 78.67, 81.71, 83.06, 77.54, 73.90, 53.89, 43.84, 70.51, 81.00, 81.60, 79.46, 79.20, 81.12, 78.34, 74.63, 56.75]

    #     mf.drawPlotRelationBtwWritecyclesAndOverheadRationAndStandardDeviation(
    #         cycle_data, overhead_ratio, standard_deviation, 200000)

    # draw plot compared by standard deviation

    #     cycle_data = [5010, 10079, 15069, 20018, 25007, 30017, 35086, 40075, 45025, 50014, 55003, 60093, 65082, 70031, 75021, 80010, 85100, 90089, 95038, 100028,
    #                   105017, 110106, 115096, 120045, 125034, 130004, 135113, 140103, 145052, 150041, 155011, 160000, 165109, 170059, 175048, 180017, 185007, 190116, 195065, 200055]
    #     sd_0 = [125.47, 76.54, 80.54, 83.76, 76.93, 80.49, 85.02, 75.99, 62.81, 46.03, 57.57, 74.28, 79.67, 83.06, 77.78, 84.66, 83.84, 76.84, 65.64,
    #             49.84, 52.07, 71.54, 83.32, 82.29, 78.67, 81.71, 83.06, 77.54, 73.90, 53.89, 43.84, 70.51, 81.00, 81.60, 79.46, 79.20, 81.12, 78.34, 74.63, 56.75]
    #     sd_1 = [372.99, 465.47, 525.04, 619.47, 660.74, 654.74, 600.15, 483.61, 439.92, 348.80, 118.30, 372.71, 445.54, 451.59, 400.79, 235.19, 302.54, 422.92, 451.81, 432.47,
    #             331.34, 176.40, 382.99, 448.13, 449.28, 391.07, 196.25, 317.89, 427.80, 450.68, 428.34, 317.89, 209.41, 391.71, 448.95, 446.91, 381.95, 159.17, 334.19, 433.33]
    #     sd_2 = [694.01, 960.29, 1141.41, 1269.57, 1370.92, 1442.31, 1493.02, 1526.23, 1534.51, 1676.31, 1799.20, 1900.13, 1982.71, 2045.93, 2095.23, 2129.29, 2151.14, 2159.99, 2154.51, 2138.01,
    #             2104.18, 2058.13, 1996.47, 1918.17, 1824.25, 1704.54, 1556.41, 1509.89, 1487.47, 1448.74, 1386.57, 1300.24, 1182.55, 1023.71, 804.06, 432.83, 552.81, 871.84, 1069.88, 1213.61]
    #     ratio0 = [1.2898, 1.3076, 1.3372, 1.3520, 1.3610, 1.3670, 1.3714, 1.3745, 1.3771, 1.3790, 1.3806, 1.3820, 1.3831, 1.3841, 1.3850, 1.3857, 1.3864, 1.3870, 1.3875,
    #               1.3880, 1.3884, 1.3888, 1.3891, 1.3895, 1.3898, 1.3900, 1.3903, 1.3905, 1.3908, 1.3910, 1.3911, 1.3913, 1.3915, 1.3917, 1.3918, 1.3919, 1.3921, 1.3922, 1.3924, 1.3925]
    #     ratio1 = [1.0659, 1.0661, 1.0630, 1.0557, 1.0512, 1.0483, 1.0461, 1.0445, 1.0463, 1.0482, 1.0498, 1.0512, 1.0524, 1.0534, 1.0543, 1.0551, 1.0556, 1.0562, 1.0568,
    #               1.0572, 1.0577, 1.0581, 1.0584, 1.0588, 1.0591, 1.0594, 1.0596, 1.0598, 1.0600, 1.0603, 1.0605, 1.0606, 1.0608, 1.0610, 1.0611, 1.0613, 1.0614, 1.0616, 1.0616, 1.0618]
    #     ratio2 = [1.0196, 1.0196, 1.0198, 1.0199, 1.0198, 1.0199, 1.0199, 1.0198, 1.0199, 1.0190, 1.0181, 1.0175, 1.0169, 1.0164, 1.0160, 1.0156, 1.0153, 1.0150, 1.0147,
    #               1.0145, 1.0143, 1.0141, 1.0139, 1.0137, 1.0136, 1.0135, 1.0133, 1.0134, 1.0137, 1.0139, 1.0141, 1.0143, 1.0144, 1.0146, 1.0147, 1.0149, 1.0150, 1.0151, 1.0152, 1.0154]

    #     mf.drawPlotByStandardDeviation(
    #         cycle_data, sd_0, sd_1, sd_2, 200000, "THVsStandardDeviation")

    #     mf.drawPlotByOverheadratio(
    #         cycle_data, ratio0, ratio1, ratio2, 200000, "THVsOverheadRatio"
    #     )

    #     mf.makeBarRAMCost([863, 863, 863], True)
    #     mf.makeBarRAMCost([22, 66, 115], True)
    #     mf.makeBarPageEC(data.page_cnt)
    #     mf.WithoutCacheVsLRUVs2Q([0.05, 0.10, 0.15, 0.20], [[
    #                              0.37, 0.52, 0.62, 0.69], [51, 42, 37, 32]], [[0.33, 0.45, 0.58, 0.66], [53, 47, 39, 34]], [[0.23, 0.31, 0.36, 0.43], [60, 57, 54, 50]], [[0.25, 0.36, 0.46, 0.51], [58, 52, 48, 43]], [68, 68, 68, 68], [68, 68, 68, 68])

    #     mf.makeBarEdgecachedpVsOrigindpVSWithoutDP([486.54, 1.0099, 387, 305], [
    #         237.20, 1.0373, 392, 2100], [21600.23, 1.0000, 423, 0], 30000, True)

    #     mf.makeBarPageEC(data.page_cnt)

    #     mf.make3BarFigurePageEC(
    #         data.page_cnt_nowl, data.page_cnt_dp, data.page_cnt_pdp)

    mf.drawPlotByStandardDeviation(
        data.PDP500_cycles, data.PDP500_standard_deviation, data.TH20_cycles, data.TH20_standard_deviation,
        data.TH50_cycles, data.TH50_standard_deviation, data.TH80_cycles, data.TH80_standard_deviation,
        data.TH110_cycles, data.TH110_standard_deviation,  "PDPVsDPByStandarddeviation")

    #     mf.drawPlotByOverheadratio(data.PDP500_cycles, data.PDP500_overhead_ratio, data.TH20_cycles, data.TH20_overhead_ratio,
    #                                data.TH50_cycles, data.TH50_overhead_ratio, data.TH80_cycles, data.TH80_overhead_ratio,
    #                                data.TH110_cycles, data.TH110_overhead_ratio,  "PDPVsDPByOverheadratio")
    #     mf.drawPlotByWritespeed(data.PDP500_cycles, data.PDP500_write_speed, data.TH20_cycles, data.TH20_write_speed,
    #                             data.TH50_cycles, data.TH50_write_speed, data.TH80_cycles, data.TH80_write_speed,
    #                             data.TH110_cycles, data.TH110_write_speed,  "PDPVsDPByWritespeed")

    #     mf.MaxCycleBtwDPAndPDP([43260, 47376, 47375, 44284, 50348])

    #     mf.drawPlotByStandardDeviation2(
    #         data.a3_cycle, data.a3_standard_deviation, data.a4_cycle, data.a4_standard_deviation,
    #         data.a5_cycle, data.a5_standard_deviation, data.a6_cycle, data.a6_standard_deviation,
    #         data.a7_cycle, data.a7_standard_deviation,  data.a72_cycle, data.a72_standard_deviation,
    #         data.a73_cycle, data.a73_standard_deviation, data.a74_cycle, data.a74_standard_deviation, "PDPDiffTHStandarddeviation")

    #     mf.MaxCycleBtwPDPTH([49949, 50428, 50348, 51205,
    #                          51070, 45229, 43924, 17023])


if __name__ == "__main__":
    main()
