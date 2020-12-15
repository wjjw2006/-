//index.js
const app = getApp()
Page({
  data: {
  },
  log_page2: function(){
    wx.redirectTo({//跳转至智能模式页面
      url: '../index2/index2',
    })
  },
  log_page3: function(){//跳转至手动模式页面
    wx.redirectTo({
      url: '../index3/index3',
    })
  },
  
  onLoad: function() {
  }
})
