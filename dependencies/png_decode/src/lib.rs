//see http://jakegoulding.com/rust-ffi-omnibus/integers/ for instructions
//see https://medium.com/jim-fleming/complex-types-with-rust-s-ffi-315d14619479

extern crate image;
extern crate libc;

use image::Pixel;

use libc::size_t;
use std::mem;

use std::io::BufReader;
use std::fs::File;

#[repr(C)]
pub struct dynamicArray<T> {
    length: size_t,
	capacity: size_t,
	ptr: *mut T,
}

#[repr(C)]
pub struct Image_rgb {
	width: size_t,
	height: size_t,
	rgb: dynamicArray<f32>,
}

#[repr(C)]
pub struct Image_seperate_channels {
	width: size_t,
	height: size_t,
	r: dynamicArray<u8>,
	g: dynamicArray<u8>,
	b: dynamicArray<u8>,
}

fn vec_to_struct<T>(mut vec: Vec<T>) -> dynamicArray<T> {
    let array = dynamicArray{ 
		length: vec.len() as size_t,
		capacity: vec.capacity() as size_t,
		ptr: vec.as_mut_ptr(),
	};
    mem::forget(vec); // prevent deallocation in Rust
                      // The array is still there but no Rust object
                      // feels responsible. We only have ptr/len now
                      // to reach it.
    array
}

///// This is intended for the C code to call for deallocating the
///// Rust-allocated i32 array.
//unsafe extern "C" fn deallocate_rust_buffer(ptr: *mut u8, capacity: size_t) {
    //let capacity = capacity as usize;
    //drop(Vec::from_raw_parts(ptr, capacity, capacity));
//}

#[no_mangle]
pub extern fn load_rgb_png() -> Image_rgb {
	let f = File::open("assets/colormap.png").unwrap();
	let reader = BufReader::new(f);
	
	let image = image::load(reader,image::ImageFormat::PNG).unwrap();
	if let image::DynamicImage::ImageRgb8(rgb_image) = image {
		println!("opened image, width: {}, height: {}",rgb_image.width(),rgb_image.height());
		
		let numb_of_pixels = (rgb_image.width()*rgb_image.height()) as usize;
		let mut rgb = Vec::with_capacity(numb_of_pixels*3*6);
		
		for pixel in rgb_image.pixels() {
			let sub_pixels = pixel.channels();
			for _ in 0..6{//6 vertices per pixel so 6 color values per pixel
				rgb.push(sub_pixels[0] as f32/255.); //red
				rgb.push(sub_pixels[1] as f32/255.); //green
				rgb.push(sub_pixels[2] as f32/255.); //blue
			}
		}
		
		Image_rgb {
			width: rgb_image.width() as size_t,
			height: rgb_image.height() as size_t,
			rgb: vec_to_struct(rgb),
		}
		
	} else {
		println!("could not load png1");
		Image_rgb {
			width: 0,
			height: 0,			
			rgb: vec_to_struct(vec!(0.0)),
		}		
		
	}
}

#[no_mangle]
pub extern fn load_channels_png() -> Image_seperate_channels {
	let f = File::open("assets/heightmap.png").unwrap();
	let reader = BufReader::new(f);
	
	let image = image::load(reader,image::ImageFormat::PNG).unwrap();
	if let image::DynamicImage::ImageRgb8(rgb_image) = image {
		println!("opened image, width: {}, height: {}",rgb_image.width(),rgb_image.height());
		
		let numb_of_pixels = (rgb_image.width()*rgb_image.height()) as usize;
		let mut r = Vec::with_capacity(numb_of_pixels);
		let mut g = Vec::with_capacity(numb_of_pixels);
		let mut b = Vec::with_capacity(numb_of_pixels);
		
		for pixel in rgb_image.pixels() {
			let sub_pixels = pixel.channels();
			r.push(sub_pixels[0]); //red
			g.push(sub_pixels[1]); //green
			b.push(sub_pixels[2]); //blue
		}
		
		Image_seperate_channels {
			width: rgb_image.width() as size_t,
			height: rgb_image.height() as size_t,
			r: vec_to_struct(r),
			g: vec_to_struct(g),
			b: vec_to_struct(b),
		}
		
	} else {
		println!("could not load png1");
		Image_seperate_channels {
			width: 0,
			height: 0,			
			r: vec_to_struct(vec!(0)),
			g: vec_to_struct(vec!(0)),
			b: vec_to_struct(vec!(0)),			
		}		
		
	}
}
