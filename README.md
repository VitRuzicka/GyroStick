# GyroStick
motion controller for model aircraft

<h1> Description</h1>
This is DIY approach similar to DJI motion controller or Vector from MotionPilot<br>
<b> Work in progress</b>



<h2>Link to ongoing development of 3d printed case</h2>
<a href="https://cad.onshape.com/documents/b3c1199856b12aa9a28603eb/w/a9a976ad83ac583be5dcaa99/e/4ac61a4f71fcc0e35afef49c">Here</a><br>
<h2>Electronics and hardware</h2> - might change further in development
<ul>
  <li><a href="https://www.aliexpress.com/item/33048962331.html?spm=a2g0o.search0303.0.0.789c553665f0a7&algo_pvid=e3233321-abd2-4204-9bb5-541e24e18778&algo_expid=e3233321-abd2-4204-9bb5-541e24e18778-0&btsid=2100bb4916143297686067991ed525&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_">TTGO T-display</a></li>
  <li><a href="https://www.aliexpress.com/item/32340949017.html?spm=a2g0o.productlist.0.0.2c543357BdyNy1&algo_pvid=2966f4e2-bcba-483c-a28e-3dbabee5240b&algo_expid=2966f4e2-bcba-483c-a28e-3dbabee5240b-0&btsid=2100bb4716143298209287501e8f63&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_">GY-521 MPU6050 Gyro</a></li>
  <li><a href="https://www.aliexpress.com/item/1900206163.html?spm=a2g0o.productlist.0.0.899a7352QIz6iH&algo_pvid=dd806e5d-f243-463e-8911-e064b97fb3b3&algo_expid=dd806e5d-f243-463e-8911-e064b97fb3b3-0&btsid=0b0a050b16143299352953017e6426&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_">49E Hall sensor</a></li>
  <li><a  href="https://www.aliexpress.com/item/1005001410127169.html?spm=a2g0o.productlist.0.0.42776a6d3o3RZ0&algo_pvid=436a74ad-7f26-45da-9015-49a9d881010a&algo_expid=436a74ad-7f26-45da-9015-49a9d881010a-0&btsid=2100bdd516143300446062313ef637&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_">5x5x2mm magnet</a> or any other, just dont forget to adjust it in onshape</li>
  <li><a href="https://www.aliexpress.com/item/1005001615918009.html?spm=a2g0o.productlist.0.0.78ab53bajwOab6&algo_pvid=f1b7cdad-a232-439f-a2d9-f30e3a957e7d&algo_expid=f1b7cdad-a232-439f-a2d9-f30e3a957e7d-5&btsid=0b0a119a16143301714608739efea4&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_">2mm rod, got mine from dvd drive</a></li>
  <li> bunch of button head m3 screws, will write down size when I try them out</li>
  <li>rubber band - small</li>
  <li><a href="https://www.aliexpress.com/item/32929601888.html?spm=a2g0o.productlist.0.0.4e9d53e7y3RyKL&algo_pvid=ef9514f6-a0c0-42ef-87aa-8a9dadeef8e8&algo_expid=ef9514f6-a0c0-42ef-87aa-8a9dadeef8e8-25&btsid=0b0a119a16143303423476306efea0&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_">18650 battery</a></li>
  
 </ul>
 <br>
 <h2>Software</h2> 
 <ul>
  <li>You would want to install ttgo t-display library from <a href="https://github.com/Xinyuan-LilyGO/TTGO-T-Display">here</a></li>
  
  </ul>
  
  <br>
  <p> I am planning to connect this gyrostick to taranis as slave device in order to keep current frsky protocol and not change any radio protocol, beacuse of this, it will be universal and usable even with flysky radios. Still thinking whether to use the DSC port 3.5mm jack or bay connector for this. If jack supports SBUS instead of ppm, I would rather leave the bay unused for XF and especially ELRS users :)
