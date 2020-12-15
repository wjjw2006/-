//index.js
const app = getApp()
Page({
  data: {
  },
  log_page2: function(){

    wx.redirectTo({
      url: '../index2/index2',
    })
  },
  log_page3: function(){
    wx.redirectTo({
      url: '../index3/index3',
    })
  },
  
  onLoad: function() {
  }
})
