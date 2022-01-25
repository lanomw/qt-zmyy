var e = require("E5CD59B5AB6B1EDF83AB31B2457EBD94.js"), o = require("A6F6C5F0AB6B1EDFC090ADF727BEBD94.js");

App({
    onLaunch: function() {
        wx.getUserInfo( {
            success(res) {
                console.log('userinfo : ', res)
                // signature e7067f62fd3ab66873091c499b2232ac8fa0381e
            },
            fail(error) {
                console.log('userinfo error : ', error)
            },
        })

        var o = this;
        e.updater(), wx.getSystemInfo({
            success: function(t) {
                console.log(t), "devtools" == t.platform && (o.NODE_ENV = "dev"), o.globalData.SDKVersion = t.SDKVersion,
                t.model && t.model.toLocaleLowerCase().match(/(windows)|(mac)|(linux)|(ubentu)|(centos)/) ? wx.showModal({
                    title: "提示",
                    content: "请使用手机访问,否则存在兼容问题",
                    showCancel: !1
                }) : e.compareVersion(t.SDKVersion, "2.13.2") < 0 && wx.showModal({
                    title: "微信版本过低",
                    content: "请将微信升级至8.0.9或以上版本",
                    showCancel: !1
                }), o.globalData.windowWidth = t.windowWidth, o.globalData.windowHeight = t.windowHeight;
            }
        }), wx.removeStorageSync("zfsw_povNotice");
    },
    NODE_ENV: "pro",
    globalData: {
        SDKVersion: "",
        userInfo: null,
        hasUserInfo: !1,
        url: "https://app.zhifeishengwu.com/",
        header: {
            Cookie: ""
        },
        windowHeight: "",
        windowWidth: "",
        personPosition: "",
        questionnaireFlag: 0,
        subscribeInfo: {
            stime: "",
            etime: "",
            mxid: "",
            customerName: "",
            customerId: "",
            birthday: "",
            tel: "",
            cname: "",
            sex: "",
            idcard: "",
            pname: "",
            pid: "",
            FTime: "",
            subscribeTime: "",
            notice: ""
        },
        positionInfo: {
            status: -2
        }
    },
    dot: function() {
        o.info("dot"), o.setFilterMsg("dot");
    },
    jump: e.jump,
    getUserInfo: e.getUserInfo,
    callSubscribeMessage: e.callSubscribeMessage
});
