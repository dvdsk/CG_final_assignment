//see http://jakegoulding.com/rust-ffi-omnibus/integers/ for instructions
//see https://medium.com/jim-fleming/complex-types-with-rust-s-ffi-315d14619479

extern crate image;
extern crate libc;

use image::Pixel;

use libc::size_t;
use libc::uint32_t;
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
pub struct pngImage {
	rgb: dynamicArray<f32>,
	a: dynamicArray<u8>,
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

/// This is intended for the C code to call for deallocating the
/// Rust-allocated i32 array.
unsafe extern "C" fn deallocate_rust_buffer(ptr: *mut u8, capacity: size_t) {
    let capacity = capacity as usize;
    drop(Vec::from_raw_parts(ptr, capacity, capacity));
}

#[no_mangle]
pub extern fn load_png() -> pngImage {
	let f = File::open("test2.png").unwrap();
	let reader = BufReader::new(f);
	
	let image = image::load(reader,image::ImageFormat::PNG).unwrap();
	if let image::DynamicImage::ImageRgba8(rgba_image) = image {
		println!("opened image, width: {}, height: {}",rgba_image.width(),rgba_image.height());
		
		let numb_of_pixels = (rgba_image.width()*rgba_image.height()) as usize;
		let mut rgb = Vec::with_capacity(numb_of_pixels*3*6);
		let mut a = Vec::with_capacity(numb_of_pixels);
		
		for pixel in rgba_image.pixels() {
			let sub_pixels = pixel.channels();
			for _ in 0..6{
				rgb.push(sub_pixels[0] as f32/255.); //red
				rgb.push(sub_pixels[1] as f32/255.); //green
				rgb.push(sub_pixels[2] as f32/255.); //blue
			}
			a.push(sub_pixels[3]);
		}
		
		pngImage {
			rgb: vec_to_struct(rgb),
			a: vec_to_struct(a),
		}
		
	} else {
		println!("could not load png1");
		pngImage {
			rgb: vec_to_struct(vec!(0.0)),
			a: vec_to_struct(vec!(0)),
		}		
		
	}
}
