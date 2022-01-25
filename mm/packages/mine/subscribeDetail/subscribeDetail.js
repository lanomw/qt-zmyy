getApp();

var t = require("../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js");

Page({
    data: {
        detail: ""
    },
    onLoad: function(t) {
        this.loadData(t.id);
    },
    onShow: function() {
        getApp().dot();
    },
    loadData: function(a) {
        var i = this;
        t.getSubcribeDetail({
            id: a
        }, function(t) {
            200 == t.data.status && i.setData({
                detail: t.data
            });
        });
    }
});