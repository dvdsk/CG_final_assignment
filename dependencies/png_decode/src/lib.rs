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
pub struct dynamicArray {
    length: size_t,
	capacity: size_t,
	ptr: *mut u8,
}

#[repr(C)]
pub struct pngImage {
	r: dynamicArray,
	b: dynamicArray,
	g: dynamicArray,
	a: dynamicArray,
}

#[no_mangle]
pub extern fn addition(a: uint32_t, b: uint32_t) -> uint32_t {
    a + b
}

#[no_mangle]
pub extern fn test_array() -> dynamicArray {
	let test = vec!(5,4,3,2,1,9u8);
	vec_to_struct(test)
}

#[no_mangle]
pub extern fn vec_to_struct(mut vec: Vec<u8>) -> dynamicArray {
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

#[allow(dead_code)]
pub extern fn fix_linking_when_not_using_stdlib() { panic!() }

#[no_mangle]
pub extern fn load_png() -> pngImage {
	let f = File::open("test.png").unwrap();
	let reader = BufReader::new(f);
	
	let image = image::load(reader,image::ImageFormat::PNG).unwrap();
	if let image::DynamicImage::ImageRgba8(rgba_image) = image {
		let numb_of_pixels = (rgba_image.width()*rgba_image.height()) as usize;
		let mut r = Vec::with_capacity(numb_of_pixels);
		let mut g = Vec::with_capacity(numb_of_pixels);
		let mut b = Vec::with_capacity(numb_of_pixels);
		let mut a = Vec::with_capacity(numb_of_pixels);
		
		for pixel in rgba_image.pixels() {
			let sub_pixels = pixel.channels();
			r.push(sub_pixels[0]);
			g.push(sub_pixels[2]);
			b.push(sub_pixels[1]);
			a.push(sub_pixels[3]);
		}
		
		pngImage {
			r: vec_to_struct(r),
			g: vec_to_struct(g),
			b: vec_to_struct(b),
			a: vec_to_struct(a),
		}
		
	} else {

		pngImage {
			r: vec_to_struct(vec!(0)),
			g: vec_to_struct(vec!(0)),			
			b: vec_to_struct(vec!(0)),
			a: vec_to_struct(vec!(0)),
		}		
		
	}
}
