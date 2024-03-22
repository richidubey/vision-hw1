# from uwimg import *
# print("Hello")
# im = load_image("data/dogsmall.jpg")
# print("Hello1")
# a = nn_resize(im, im.w*4, im.h*4)
# print("Hello2")
# save_image(a, "dog4x-nn")
# print("Hello3")

# from uwimg import *
# im = load_image("data/dogsmall.jpg")
# a = bilinear_resize(im, im.w*4, im.h*4)
# # save_image(a, "dog4x-bl")

# from uwimg import *
# im = load_image("data/dog.jpg")
# print("Making 7 times smaller")
# a = nn_resize(im, im.w//7, im.h//7)
# save_image(a, "dog7th-bl")

# print("Testing convolution by blurring the image using a box filter of size 7")
# from uwimg import *
# im = load_image("data/dog.jpg")
# f = make_box_filter(7)
# blur = convolve_image(im, f, 1)
# save_image(blur, "dog-box7")


# print("Using nn interpolation to downsize an image after applying a 7x7 box filter on it.")
# from uwimg import *
# im = load_image("data/dog.jpg")
# f = make_box_filter(7)
# blur = convolve_image(im, f, 1)
# thumb = nn_resize(blur, blur.w//7, blur.h//7)
# save_image(thumb, "dogthumb")




# print("Checking high pass filter")
# from uwimg import *
# im = load_image("data/dog.jpg")
# f = make_highpass_filter();
# conv = convolve_image(im, f, 0)
# save_image(conv, "dog_hpass")

# print("Checking sharpen filter")
# from uwimg import *
# im = load_image("data/dog.jpg")
# f = make_sharpen_filter();
# conv = convolve_image(im, f, 1)
# save_image(conv, "dog_sharpened")

# print("Checking emboss filter")
# from uwimg import *
# im = load_image("data/dog.jpg")
# f = make_emboss_filter();
# conv = convolve_image(im, f, 1)
# save_image(conv, "dog_emboss")


print("Gaussian blurring an image")
from uwimg import *
im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-gauss21")