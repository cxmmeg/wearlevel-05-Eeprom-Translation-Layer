# -*- coding: utf-8 -*-
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
        rects3 = plt.bar(x + width*3/2, {without_dp_data[i]},
                         width=width, label='without dualpool', color='#C0C0C0')
        autolabel(rects1, ax)
        autolabel(rects2, ax)
        autolabel(rects3, ax)
        plt.xlim(-1, 1)
        plt.ylim(
            top=max(edgecache_dp_data[i], orgin_dp_data[i], without_dp_data[i])*1.5)
        ax.set_xticks(x)
        ax.set_xticklabels([titles[i]])
        plt.subplots_adjust(wspace=subplots_padding, hspace=subplots_padding)
        ax.set_ylabel(yaxes_lables[i], labelpad=1.5)

    # add legend
    plt.subplot(2, 2, 2)
    plt.subplots_adjust(top=0.8)
    plt.legend(bbox_to_anchor=(0.1, 1.5), loc=2, borderaxespad=0)

    # add main title
    plt.suptitle('after ' + str(total_write_cycles) + ' page write cycles')

    plt.style.use("ggplot")

    if save:
        plt.savefig('edgecacheVSorginVSwithoutdualpool.png')
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
