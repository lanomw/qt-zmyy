var i = require("../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js");

Page({
    data: {
        tip: {},
        id: "",
        refuseTitle: "",
        refuseReson: []
    },
    onLoad: function(i) {
        var t = this;
        wx.getStorage({
            key: "questionnaire_covid19",
            success: function(s) {
                var e = s.data;
                console.log(e.list[1].QuestionTitle), t.setData({
                    tip: e,
                    id: i.id,
                    refuseTitle: e.list[1].QuestionTitle,
                    refuseReson: e.list[1].Foptions
                });
            }
        });
    },
    onShow: function() {
        getApp().dot();
    },
    bindRefuse: function(i) {
        var t = this.data.tip.list[1];
        this.dosubmit([ {
            mxid: t.mxid,
            id: t.id,
            val: t.Foptions[i.detail.value].id
        } ]);
    },
    apply: function() {
        var i = this.data.tip.list[0];
        this.dosubmit([ {
            mxid: i.mxid,
            id: i.id,
            val: i.Foptions[0].id
        } ], "/packages/classification/covid19/covid19");
    },
    dosubmit: function(t, s) {
        i.setQuestionnaireCovid19(t, function(i) {
            200 == i.data.status && (s ? wx.navigateTo({
                url: s
            }) : wx.navigateBack());
        });
    }
});