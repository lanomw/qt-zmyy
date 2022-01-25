var t = require("../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js");

Page({
    data: {
        btns: [ {
            label: "详情",
            class: "card-bt card-bt-full-blue"
        }, {
            label: "取消预约",
            class: "card-bt"
        } ],
        tip: "过时将取消预约",
        list: []
    },
    onShow: function() {
        getApp().dot(), this.loadData();
    },
    onclick: function(a) {
        var l = this;
        "取消预约" == a.detail.label ? wx.showModal({
            title: "提示",
            content: "是否确认取消预约",
            success: function(s) {
                s.confirm && t.cancelScribe({
                    id: a.detail.id
                }, function(t) {
                    200 == t.data.status ? (wx.showToast({
                        title: "取消成功",
                        duration: 1e3
                    }), l.loadData()) : wx.showModal({
                        title: "提示",
                        content: t.data.msg,
                        showCancel: !1
                    });
                });
            }
        }) : "我已接种" == a.detail.label ? wx.showModal({
            title: "提示",
            content: "未接种，请不要点击",
            success: function(s) {
                s.confirm && t.comfirmVaccin({
                    id: a.detail.id
                }, function(t) {
                    200 == t.data.status ? (wx.showToast({
                        title: "操作成功",
                        duration: 1e3
                    }), l.loadData()) : wx.showModal({
                        title: "提示",
                        content: t.data.msg,
                        showCancel: !1
                    });
                });
            }
        }) : "详情" == a.detail.label && wx.navigateTo({
            url: "/packages/mine/subscribeDetail/subscribeDetail?id=" + a.detail.id
        });
    },
    loadData: function() {
        var a = this, l = [];
        t.getSubcribeList(function(t) {
            200 == t.data.status ? (t.data.list.forEach(function(t) {
                var a = t.VaccineDate.split(" ");
                a = a[0] + " " + a[1].split("-")[1], t.tip = '<span class="iconfont card-tip-icon iconlingdang card-tip-text" style="font-size: 12px;"></span><span class="card-tip-text">过时将取消预约</span>';
                var s = "已预约";
                t.isFinished ? (s = "已接种", t.btns = [ {
                    id: t.id,
                    label: "详情",
                    class: "card-bt card-bt-full-blue"
                } ]) : t.isCancel ? (s = "已取消", t.btns = [ {
                    id: t.id,
                    label: "详情",
                    class: "card-bt card-bt-full-blue"
                } ]) : t.finishedBtn ? t.btns = [ {
                    id: t.id,
                    label: "我已接种",
                    class: "card-bt card-bt-full-blue"
                }, {
                    id: t.id,
                    label: "取消预约",
                    class: "card-bt"
                }, {
                    id: t.id,
                    label: "详情",
                    class: "card-bt"
                } ] : t.btns = [ {
                    id: t.id,
                    label: "详情",
                    class: "card-bt card-bt-full-blue"
                }, {
                    id: t.id,
                    label: "取消预约",
                    class: "card-bt"
                } ], t.status = s, l.push(t);
            }), a.setData({
                list: l
            })) : a.setData({
                list: []
            }), wx.stopPullDownRefresh();
        });
    },
    onPullDownRefresh: function() {
        this.loadData();
    }
});