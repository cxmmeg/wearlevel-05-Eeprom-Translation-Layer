# -*- coding: utf-8 -*-

from matplotlib.pyplot import MultipleLocator
import matplotlib.pyplot as plt
import numpy as np
import random
from matplotlib import cm
from matplotlib import axes


def autolabel(rects, ax):
    """Attach a text label above each bar in *rects*, displaying its height."""
    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')


def makeBarEdgecachedpVsOrigindp(edgecache_dp_data, orgin_dp_data, total_write_cycles, save):

    # +++++++++++++++++++++++params+++++++++++++++++++++++++++

    titles = ['standard deviation',
              'overhead ratio', 'write speed', 'ram cost']

    yaxes_lables = ['', '', 'byte/s', 'byte']

#     edgecache_dp_data = [156.8, 1.06, 101, 584]
#     orgin_dp_data = [129.62, 1.06, 136, 1356]

    x = np.arange(1)  # the label locations
    width = 0.4  # the width of the bars

    subplots_padding = 0.4

    # ----------------------params---------------------------

    plt.figure(1, dpi=600)
    plt.title('after 46896 page write cycles')

    for i in range(4):
        ax = plt.subplot(2, 2, i+1)
        rects1 = plt.bar(x - width/2, {edgecache_dp_data[i]},
                         width=width, label='edge-cache dualpool', color='#0099FF')
        rects2 = plt.bar(x + width/2, {orgin_dp_data[i]},
                         width=width, label='origin dualpool', color='#FF9900')
        autolabel(rects1, ax)
        autolabel(rects2, ax)
        plt.xlim(-1, 1)
        plt.ylim(top=max(edgecache_dp_data[i], orgin_dp_data[i])*1.5)
        ax.set_xticks(x)
        ax.set_xticklabels([titles[i]])
        plt.subplots_adjust(wspace=subplots_padding, hspace=subplots_padding)
        ax.set_ylabel(yaxes_lables[i], labelpad=1.5)

    # add legend
    plt.subplot(2, 2, 2)
    plt.subplots_adjust(top=0.8)
    plt.legend(bbox_to_anchor=(0.1, 1.4), loc=2, borderaxespad=0)

    # add main title
    plt.suptitle('after ' + str(total_write_cycles) + ' page write cycles')

    plt.style.use("ggplot")

    if save:
        plt.savefig('edgecacheVSorgin.png')
    else:
        plt.show()


def makeBarEdgecachedpVsOrigindpVSWithoutDP(edgecache_dp_data, orgin_dp_data, without_dp_data, total_write_cycles, save):

    # +++++++++++++++++++++++params+++++++++++++++++++++++++++

    titles = ['standard deviation',
              'overhead ratio', 'write speed', 'ram cost']

    yaxes_lables = ['标准差', '负载比', '平均写速度(byte/s)', 'RAM资源开销(byte)']

#     edgecache_dp_data = [156.8, 1.06, 101, 584]
#     orgin_dp_data = [129.62, 1.06, 136, 1356]

    x = np.arange(1)  # the label locations
    width = 0.4  # the width of the bars

    subplots_padding = 0.4

    # ----------------------params---------------------------

    plt.figure(1, dpi=600)
#     plt.title('after 46896 page write cycles')
    plt.rcParams['font.sans-serif'] = ['Arial Unicode MS']

    for i in range(4):
        ax = plt.subplot(2, 2, i+1)
        rects1 = plt.bar(x - width/2, {edgecache_dp_data[i]},
                         width=width, label='渐进式双池算法', color='#0099FF')
        rects2 = plt.bar(x + width/2, {orgin_dp_data[i]},
                         width=width, label='双池算法', color='#FF9900')
        rects3 = plt.bar(x + width*3/2, {without_dp_data[i]},
                         width=width, label='无磨损均衡介入', color='#C0C0C0')
        autolabel(rects1, ax)
        autolabel(rects2, ax)
        autolabel(rects3, ax)
        plt.xlim(-1, 1)
        plt.ylim(
            top=max(edgecache_dp_data[i], orgin_dp_data[i], without_dp_data[i])*1.5)
        ax.set_xticks(x)
        ax.set_xticklabels('')
        plt.subplots_adjust(wspace=subplots_padding, hspace=subplots_padding)
        ax.set_ylabel(yaxes_lables[i], labelpad=1.5)

    # add legend
    plt.subplot(2, 2, 2)
    plt.subplots_adjust(top=0.8)
    plt.ylim(1.0, 1.1)
    plt.legend(bbox_to_anchor=(0.2, 1.5), loc=2, borderaxespad=0)

    # add main title
#     plt.suptitle('after ' + str(total_write_cycles) + ' page write cycles')

    plt.style.use("ggplot")

    if save:
        plt.savefig('PDPVsDPVsNOWL.png')
    else:
        plt.show()


def drawHot(xlen, ylen, data, total_write_cycles, savename, save):

    xlables = []
    ylables = []
    for i in range(xlen):
        xlables.append(str(i))
    for i in range(ylen):
        ylables.append(str(i))

    # 作图阶段
    fig = plt.figure(dpi=600)
    # 定义画布为1*1个划分，并在第1个位置上进行作图
    ax = fig.add_subplot(111)
    # 定义横纵坐标的刻度
    ax.set_yticks([])
    ax.set_xticks([])
    # 作图并选择热图的颜色填充风格，这里选择hot
    im = ax.imshow(data, cmap=plt.cm.cool)
    # 增加右侧的颜色刻度条
    plt.colorbar(im)
    # 增加标题
    plt.title('after ' + str(total_write_cycles) + ' page write cycles')

    if save:
        plt.savefig(savename)
    else:
        plt.show()


def drawPlotRelationBtwWritecyclesAndOverheadRationAndStandardDeviation(cyclesData, OverheadRatioData, StandardDeviationData, cycles):

    l1 = plt.plot(cyclesData, OverheadRatioData, 'r--', label='overhead ratio')
    l2 = plt.plot(cyclesData, StandardDeviationData,
                  'g--', label='standard deviation')
    plt.plot(cyclesData, OverheadRatioData, 'ro-',
             cyclesData, StandardDeviationData, 'g+-')
    plt.title('after ' + str(cycles) + ' cycles page write')
    plt.xlabel('page write cycles')
#     plt.ylabel('column')
    plt.legend()
    plt.show()


def drawPlotByStandardDeviation(cycleData0, StandardDeviationData0, cycleData1, StandardDeviationData1, cycleData2, StandardDeviationData2,
                                cycleData3, StandardDeviationData3, cycleData4, StandardDeviationData4, saveName):

    plt.rcParams['font.sans-serif'] = ['Arial Unicode MS']
    plt.figure(dpi=600)
    l1 = plt.plot(cycleData0, StandardDeviationData0,
                  'o-', label='PDP')
    l2 = plt.plot(cycleData1, StandardDeviationData1,
                  '+-', label='DP TH=20')
    l3 = plt.plot(cycleData2, StandardDeviationData2,
                  '^-', label='DP TH=50')
    l3 = plt.plot(cycleData3, StandardDeviationData3,
                  '*-', label='DP TH=80')
    l3 = plt.plot(cycleData4, StandardDeviationData4,
                  'd-', label='DP TH=110')
#     plt.plot(cycleData, StandardDeviationData0, 'ro-',
#              cycleData, StandardDeviationData1, 'g+-',
#              cycleData, StandardDeviationData2, 'b^-'
#              )
    plt.xlabel('页写入操作请求数')
    plt.ylabel('标准差')
    plt.legend()
    plt.axis([0, 51000, 0, 1500])

    if len(saveName) != 0:
        plt.savefig(saveName)
    else:
        plt.show()


def drawPlotByOverheadratio(cycleData0, StandardDeviationData0, cycleData1, StandardDeviationData1, cycleData2, StandardDeviationData2,
                            cycleData3, StandardDeviationData3, cycleData4, StandardDeviationData4, saveName):

    plt.rcParams['font.sans-serif'] = ['Arial Unicode MS']
    plt.figure(dpi=600)
    l1 = plt.plot(cycleData0, StandardDeviationData0,
                  'o-', label='PDP')
    l2 = plt.plot(cycleData1, StandardDeviationData1,
                  '+-', label='DP TH=20')
    l3 = plt.plot(cycleData2, StandardDeviationData2,
                  '^-', label='DP TH=50')
    l3 = plt.plot(cycleData3, StandardDeviationData3,
                  '*-', label='DP TH=80')
    l3 = plt.plot(cycleData4, StandardDeviationData4,
                  'd-', label='DP TH=110')
#     plt.plot(cycleData, StandardDeviationData0, 'ro-',
#              cycleData, StandardDeviationData1, 'g+-',
#              cycleData, StandardDeviationData2, 'b^-'
#              )
    plt.xlabel('页写入操作请求数')
    plt.ylabel('负载比')
    plt.legend()
    plt.axis([0, 51000, 1.0, 1.15])

    if len(saveName) != 0:
        plt.savefig(saveName)
    else:
        plt.show()


def makeBarRAMCost(ram_cost, save):

    # +++++++++++++++++++++++params+++++++++++++++++++++++++++

    x = np.arange(1)  # the label locations
    width = 0.4  # the width of the bars

    subplots_padding = 0.4

    # ----------------------params---------------------------

    plt.figure(1, dpi=600)
    plt.title('Write Speed')

    ax = plt.subplot(1, 1, 1)
    rects1 = plt.bar(x - width/2, {ram_cost[0]},
                     width=width, label='thresh hold = 5', color='#0099FF')
    rects2 = plt.bar(x + width/2, {ram_cost[1]},
                     width=width, label='thresh hold = 30', color='#FF9900')
    rects3 = plt.bar(x + width*3/2, {ram_cost[2]},
                     width=width, label='thresh hold = 100', color='#C0C0C0')
    autolabel(rects1, ax)
    autolabel(rects2, ax)
    autolabel(rects3, ax)
    plt.xlim(-0.7, 1.1)
    plt.ylim(
        top=max(ram_cost[0], ram_cost[1], ram_cost[2])*1.5)
    ax.set_xticks([])
#     ax.set_xticklabels([titles[i]])
    plt.subplots_adjust(wspace=subplots_padding, hspace=subplots_padding)
    ax.set_ylabel('Bytes/s', labelpad=1.5)

    plt.legend()

    plt.style.use("ggplot")

    if save:
        plt.savefig('speed.png')
    else:
        plt.show()


def make3BarFigurePageEC(pageECListNOWL, pageECListDP, pageECListPDP):

    #     plt.figure(1, dpi=600)
    plt.rcParams['font.sans-serif'] = ['Arial Unicode MS']

    ax = plt.subplot(1, 2, 1)
    rects = plt.bar(range(len(pageECListDP)),
                    pageECListDP,  color='#0099FF')
    ax.set_ylabel('擦写周期数')
    ax.set_xlabel('物理页号')
    plt.title('双池算法')
    ax = plt.subplot(1, 2, 2)
    rects = plt.bar(range(len(pageECListPDP)),
                    pageECListPDP,  color='#0099FF')
    ax.set_ylabel('擦写周期数')
    ax.set_xlabel('物理页号')
    plt.title('渐进式双池算法')

    plt.tight_layout()
#     plt.show()
    plt.savefig('DPVsPDP_页面磨损分布.png')


def makeBarPageEC(pageECList):

    plt.figure(1, dpi=600)
    plt.rcParams['font.sans-serif'] = ['Arial Unicode MS']
    ax = plt.subplot(1, 1, 1)
    rects = plt.bar(range(len(pageECList)), pageECList, color='#0099FF')
    ax.set_ylabel('擦写周期数')
    ax.set_xlabel('物理页号')
    plt.title('无磨损均衡介入')

#     plt.show()
    plt.savefig('无磨损均衡页面分布.png')


def WithoutCacheVsLRUVs2Q(cacheratio,  t1_lru, t1_2q, t2_lru, t2_2q, t1_NOCMT, t2_NOCMT):

    plt.rcParams['font.sans-serif'] = ['Arial Unicode MS']
    plt.figure(2, dpi=600)

    ax = plt.subplot(2, 1, 1)
    plt.plot(cacheratio, t1_lru[0], 'o--', label='T1 LRU-CMT')
    plt.plot(cacheratio, t1_2q[0], '^--', label='T1 2Q-CMT')
    plt.plot(cacheratio, t2_lru[0], 'o--', label='T2 LRU-CMT')
    plt.plot(cacheratio, t2_2q[0], '^--', label='T2 2Q-CMT')
    plt.xlabel('缓存映射表容量占全局页表大小的比例')
    plt.ylabel('缓存映射表命中率')
    ax.xaxis.set_major_locator(MultipleLocator(0.05))

    ax = plt.subplot(2, 1, 2)
    plt.plot(cacheratio, t1_lru[1], 'o--', label='T1 LRU-CMT')
    plt.plot(cacheratio, t1_2q[1], '^--', label='T1 2Q-CMT')
    plt.plot(cacheratio, t2_lru[1], 'o--', label='T2 LRU-CMT')
    plt.plot(cacheratio, t2_2q[1], '^--', label='T2 2Q-CMT')
    plt.plot(cacheratio, t1_NOCMT, '+--', label='T1/T2 without CMT')
    plt.xlabel('缓存映射表容量占全局页表大小的比例')
    plt.ylabel('运行时间 sec')
    ax.xaxis.set_major_locator(MultipleLocator(0.05))

#     plt.subplot(2, 1, 1)
    plt.tight_layout()
    plt.subplots_adjust(top=0.75)
    plt.legend(bbox_to_anchor=(0, 3.25), loc=2)
    plt.savefig('CMT-performance.png')
#     plt.show()
