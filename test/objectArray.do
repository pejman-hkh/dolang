main() {
	let o = {}
	let i = {}
	i.i = 0
	o.l = []

	o.l[i.i++] = i
	o.l[i.i++] = i
	o.l[i.i++] = i

	print( o.l[i.i] )


}