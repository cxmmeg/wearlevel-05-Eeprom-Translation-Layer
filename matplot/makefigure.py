# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import numpy as np


def autolabel(rects, ax):
    """Attach a text label above each bar in *rects*, displaying its height."""
    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')


def makeBarEdgecachedpVsOrigindp(edgecache_dp_data, orgin_dp_data, total_write_cycles):

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

    plt.figure(1)
    plt.title('after 46896 page write cycles')

    for i in range(4):
        ax = plt.subplot(2, 2, i+1)
        rects1 = plt.bar(x - width/2, {edgecache_dp_data[i]},
                         width=width, label='edge-cache dualpool')
        rects2 = plt.bar(x + width/2, {orgin_dp_data[i]},
                         width=width, label='origin dualpool')
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

    plt.suptitle('after ' + str(total_write_cycles) + 'page write cycles')

    plt.show()
