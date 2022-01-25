require("../../../EE492EF5AB6B1EDF882F46F2459EBD94.js"), getApp();

Component({
    behaviors: [],
    properties: {
        content: String,
        show: Boolean
    },
    data: {
        scrollTop: 0
    },
    lifetimes: {
        attached: function() {},
        moved: function() {},
        detached: function() {}
    },
    attached: function() {},
    ready: function() {},
    observers: {
        show: function() {}
    },
    pageLifetimes: {
        show: function() {}
    },
    methods: {
        close: function() {
            this.setData({
                show: !1
            });
        }
    }
});