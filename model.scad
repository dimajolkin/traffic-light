
 module light(h=200, r=180) {
     x=h/2;
     
    rotate([0, 90, 0])   
    translate([0, 0, -x])
    cylinder(h=h, r1=r, r2=0, center=true);    
     
     rotate([0, -90, 0])   
    translate([0, 0, -x])
    cylinder(h=h, r1=r, r2=0, center=true);    
     
      
     rotate([90, 0, 0])   
    translate([0, 0, -x])
    cylinder(h=h, r1=r, r2=0, center=true);    
      
     rotate([270, 0, 0])   
    translate([0, 0, -x])
    cylinder(h=h, r1=r, r2=0, center=true);  
}



module box(h) {
    difference() {
             cube([h, h, h], center=true);
             light(h=h, r=h - 20);  
          }
}

module hope(h=150, d=30) {
    cylinder(h=h, r1=10, r2=10);
    sphere(d=d);
}


module fingers(h, dx=15, up=1, delta=0) {
    cx = h/2;
    cy = h/2;
    cz = h/2 * up;
    
    translate([cx - dx, cy - dx, cz - 1])
    cylinder(h=6, r1=5, r2=5);
        
    translate([cx - dx, -cy + dx, cz - 1])
    cylinder(h=6, r1=5, r2=5);
    
        
    translate([-cx + dx, cy - dx, cz - 1])
    cylinder(h=6, r1=5, r2=5);
        
    translate([-cx + dx, -cy + dx, cz - 1])
    cylinder(h=6, r1=5, r2=5);
}
 
 

module trafficBox(h) {
     fingers(h, up=1, delta=0.3);
  
     difference() {
         box(h=h);
         hope(h=h, d=30);
         fingers(h=h, up=-1);
         windows(h);
         cylinder(h=h+10, r1=10, r2=10, center=true);
    }
    
 }
 
module head(h) {
rotate([0, 0, 45]) 
cylinder(50, h,h - 50,$fn=4); 
}



module windows(h, dx=20, dy=5) {
translate([0, h/2 - dy, 0])
cube([h - dx, 1, h + dx], center=true);

translate([0, -h/2 + dy, 0])
cube([h - dx, 1, h + dx], center=true);
    
rotate([0, 0, 90]) 
translate([0, -h/2 + dy, 0])
cube([h - dx, 1, h + dx], center=true);
    
rotate([0, 0, 90]) 
translate([0, h/2 - dy , 0])
cube([h - dx, 1, h + dx], center=true);
}


module power_v12(h, h_bottom, h_floor) {
  color("red")
  translate([h/2 - 5, 0, -h_bottom/2 + h_floor + 2]) //2 mm 
  cube([8, 10, 12]);
}
    
module power_usb(h, h_bottom, h_floor=3) {
    color("blue")
    translate([h/2 - 5, -24 - 9,  -h_bottom/2 + h_floor + 2])
    cube([8, 12, 8]);
}
    
module one_fastener(h, h_bottom=30, h_floor=3, h_wall, dx=1, dy=1) {
    difference() {
      translate([dx * (h/2 - h_wall), dy * (h/2 - h_wall), 0])
      cube([5, 5, h_bottom], center=true);
        
      color("yellow")
      translate([dx * (h/2 - h_wall), dy * (h/2 - h_wall), -20])
      cylinder(h_bottom + 10, 1 ,1, $fn=30);
    }
}

module one_hope_fastener(h, h_bottom, h_wall, dx, dy) {
         color("yellow")
         translate([dx * (h/2 - h_wall), dy * (h/2 - h_wall), -20])
         cylinder(h_bottom + 10, 1 ,1, $fn=30);
}

module fastener(h, h_bottom=30, h_floor=3, h_wall) {
    one_fastener(h=h, h_bottom=h_bottom, h_floor=h_floor, h_wall=h_wall, dx=1, dy=1);
    one_fastener(h=h, h_bottom=h_bottom, h_floor=h_floor, h_wall=h_wall, dx=-1, dy=1);
    one_fastener(h=h, h_bottom=h_bottom, h_floor=h_floor, h_wall=h_wall, dx=1, dy=-1);
    one_fastener(h=h, h_bottom=h_bottom, h_floor=h_floor, h_wall=h_wall, dx=-1, dy=-1);
}

module bottom(h, h_bottom=30, h_floor=3) {
     h_wall = 3;
        
     fastener(h=h, h_bottom=h_bottom, h_floor=h_floor, h_wall=h_wall);
     difference() {
         cube([h, h, h_bottom], center=true);
         
         translate([0, 0, h_floor])
         cube([h - h_floor, h - h_floor, h_bottom], center=true);
         power_usb(h=h, h_bottom=h_bottom, h_floor=h_floor);
         power_v12(h=h, h_bottom=h_bottom, h_floor=h_floor);

         one_hope_fastener(h=h, h_bottom=h_bottom, h_wall=h_wall, dx=1, dy=1);
         one_hope_fastener(h=h, h_bottom=h_bottom, h_wall=h_wall, dx=-1, dy=1);
         one_hope_fastener(h=h, h_bottom=h_bottom, h_wall=h_wall, dx=1, dy=-1);
         one_hope_fastener(h=h, h_bottom=h_bottom, h_wall=h_wall, dx=-1, dy=-1);
     }
     
    
}


module body() {
h_bottom=30;    
//translate([0, 0, h])
//trafficBox(h=h);
//trafficBox(h=h);  

translate([0, 0, -h/2 - h_bottom/2])
bottom(h, h_bottom=h_bottom);

   
    }

module main(h=200) {
   difference() {
       body(h);
       //windows(h);
       } 
    }
    
    

h=100;    
main(h);


//rotate([0, 0, 90]) 
//translate([5, 0, 5])
//cube([90, 1, 100], center=true);

//translate([0, 0, h + h/2])
//head(h);
//translate([0, 0, h])
//trafficUp(h=h);
//trafficDown(h=h);
 
 
 

