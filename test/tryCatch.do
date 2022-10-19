main() {

	console.log('test\n')

	try {
		nonExistentFunction();
	} catch (error) {
		console.error(error)
	}

}