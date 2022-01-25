require("../../../../EE492EF5AB6B1EDF882F46F2459EBD94.js");

var a = require("../../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js"), i = getApp();

Page({
    data: {
        qForm: {},
        questionnaireId: "",
        qInfo: ""
    },
    onLoad: function(a) {
        if (a.pid && a.pname && a.IdcardLimit) i.globalData.subscribeInfo.pid = a.pid, i.globalData.subscribeInfo.pname = a.pname, 
        i.globalData.subscribeInfo.IdcardLimit = a.IdcardLimit; else if (!i.globalData.subscribeInfo.pid || i.globalData.subscribeInfo.pname || i.globalData.subscribeInfo.IdcardLimit) return void this.setData({
            days: []
        });
        this.setData({
            questionnaireId: a.questionnaireId
        }), this.initForm();
    },
    onShow: function() {
        getApp().dot();
    },
    initForm: function(i) {
        var t = this;
        a.getQuestionnaire({
            id: this.data.questionnaireId
        }, function(a) {
            if (200 == a.data.status) {
                var i = [];
                a.data.list.forEach(function(a) {
                    "date" == a.Ftype ? i.push({
                        id: a.id,
                        mxid: a.mxid,
                        val: ""
                    }) : "option" == a.Ftype && i.push({
                        id: a.id,
                        mxid: a.mxid,
                        val: 0
                    });
                }), t.setData({
                    qInfo: a.data,
                    qForm: i
                });
            }
        });
    },
    bindHistoryChange: function(a) {
        var i = this.data.qForm, t = a.currentTarget.dataset;
        i[t.index] = {
            id: t.id,
            mxid: t.mxid,
            val: a.detail.value
        }, this.setData({
            qForm: i
        });
    },
    bindRadio: function(a) {
        var i = this.data.qForm, t = a.currentTarget.dataset;
        i[t.index] = {
            id: t.id,
            mxid: t.mxid,
            val: t.value
        }, this.setData({
            qForm: i
        });
    },
    bindSubmit: function() {
        var t = this;
        a.setQuestionnaire(this.data.qForm, function(a) {
            200 == a.data.status ? (i.globalData.questionnaireFlag = t.data.questionnaireId, 
            wx.redirectTo({
                url: "/packages/pov/pages/subscribeDate/subscribeDate?pid=" + i.globalData.subscribeInfo.pid + "&pname=" + i.globalData.subscribeInfo.pname + "&IdcardLimit=" + i.globalData.subscribeInfo.IdcardLimit
            })) : wx.showModal({
                title: "提交失败",
                content: a.data.msg,
                showCancel: !1
            });
        });
    }
});