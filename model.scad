
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



module fingers(h, dx=15, up=1) {
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
 
 
module trafficUp(h) {
    difference() {
    difference() {   
          difference() {
          box(h=h);
         hope(h=h/2+ 10);
          }

        cylinder(h=h+10, r1=10, r2=10, center=true);
    }
    fingers(h=h, up=-1);
}
 }
   
 

module trafficDown(h) {
     fingers(h);
     difference() {
         box(h=h);
         hope(h=h, d=30);
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


module body() {
    
//translate([0, 0, h])
//trafficUp(h=h);
trafficDown(h=h);  
    }

module main(h=200) {
   difference() {
       body(h);
       windows(h);
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
 
 
 

