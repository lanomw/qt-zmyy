Page({
    data: {
        notice: ""
    },
    onLoad: function(n) {
        this.setData({
            notice: getApp().globalData.subscribeInfo.notice
        });
    },
    onReady: function() {},
    onShow: function() {},
    onHide: function() {},
    onUnload: function() {},
    onPullDownRefresh: function() {},
    onReachBottom: function() {},
    onShareAppMessage: function() {}
});