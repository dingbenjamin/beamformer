import numpy as np

class LmsBeamformer:
    """
    An LMS beamformer for a uniform linear array.
    """

    def __init__(self, number_of_elements, training_signal, step_type, step_size=0.5):
        self.number_of_elements = number_of_elements
        self.training_signal = training_signal
        self.step_type = step_type
        self.step_size = step_size
        self.weights_estimate = np.zeros((number_of_elements, 1))
        self.autocorrelation_estimate = np.zeros((number_of_elements, 1))
        self.num_snapshots = 0

    def update_weights(self, snapshot):
        self.num_snapshots += 1
        output_estimate = np.matmul(snapshot.conj().T,self.weights_estimate)
        error = self.training_signal[self.num_snapshots%(len(self.training_signal)-1)]-output_estimate
        self.update_autocorrelation(snapshot)
        step = 0.0005*2/np.trace(self.autocorrelation_estimate) if self.step_type == 'adaptive' else self.step_size
        self.weights_estimate = self.weights_estimate + step*np.matmul(snapshot,error)

        return (self.weights_estimate, output_estimate)

    def update_autocorrelation(self, snapshot):
        self.autocorrelation_estimate = (1 - 1/self.num_snapshots)*self.autocorrelation_estimate + \
                                        1/self.num_snapshots * np.matmul(snapshot, snapshot.conj().T)