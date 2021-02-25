import tensorflow as tf

class PRelu(tf.Module):
    def __init__(self, num_features: int):
        """An activation operation whose level of non-linearity can be 
           controlled by learnable parameters.

        Args:
            num_features (int): The number of hidden units needed to be 
                put into this activation function.
        """
        self.alphas = tf.Variable(
            name="relu_params",
            initial_value=0.25*tf.ones(
                shape=[num_features], dtype=tf.float32))

    @tf.function
    def __call__(self, features):
        pos = tf.maximum(x=features, y=0)
        neg = tf.minimum(x=features, y=0)
        return pos + self.alphas*neg

class FeaturePlanesBuilder(tf.Module):
    def __init__(self, input_size: int):
        """It converts the game states into a structure ingestable by 
           the convolution operation.

        Args:
            input_size (int): The game's board size.
        """
        self.input_size_ = input_size

    @tf.function
    def __call__(self,
                 boards: tf.Tensor,
                 game_phases: tf.Tensor,
                 next_move_stone_types: tf.Tensor) -> tf.Tensor:
        """Converts the game states into a list of 2D feature planes and 
           stacks them as a multi-channel image.

        Args:
            boards (tf.Tensor): A uint8 tensor of shape 
                [BATCH_SIZE, INPUT_SIZE, INPUT_SIZE] containing the board features. 
                In particular, 
                    a black stone should take the value 1; 
                    white stone should take the value 2;
                    empty position is coded as 0.
            game_phases (tf.Tensor): A 1D uint8 tensor representing the game phase.
                Each game phase should take the following values:
                    Place 3 stones: 0
                    Swap 2 decision: 1
                    Place 2 more stones: 2
                    Stone type decision: 3
                    Standard Gomoku: 4
            next_move_stone_types (tf.Tensor): A 1D uint8 tensor representing the 
                stone type to be put on the Gomoku board. Each stone type will take 
                the following values:
                    If no stone involves in the next action, 
                        then it should be marked as 0.
                    Black stone takes the value 1.
                    White stone takes the value 2.

        Returns:
            tf.Tensor: A float32 image with 10 channels.
        """
        black_stones = tf.equal(
            x=boards, y=1, name="black_stones_extraction")
        black_stones = tf.cast(x=black_stones, dtype=tf.float32) # pylint: disable=no-value-for-parameter, unexpected-keyword-arg
        black_stones = tf.reshape(
            tensor=black_stones, 
            shape=[-1, self.input_size_, self.input_size_, 1],
            name="black_stones_create_channel")

        white_stones = tf.equal(
            x=boards, y=2, name="white_stones_extraction")
        white_stones = tf.cast(x=white_stones, dtype=tf.float32) # pylint: disable=no-value-for-parameter, unexpected-keyword-arg
        white_stones = tf.reshape(
            tensor=white_stones,
            shape=[-1, self.input_size_, self.input_size_, 1],
            name="white_stones_create_channel")
        
        one_hot_game_phases = tf.one_hot( # pylint: disable=no-value-for-parameter
            indices=game_phases, depth=5, name="one_hot_expand_game_phases")
        one_hot_game_phases = tf.reshape(
            tensor=one_hot_game_phases, 
            shape=[-1, 1, 1, 5],
            name="game_phases_create_channels")
        one_hot_game_phases = tf.repeat(
            input=one_hot_game_phases, 
            repeats=[self.input_size_], 
            axis=2,
            name="game_phase_plane_height")
        one_hot_game_phases = tf.repeat(
            input=one_hot_game_phases, 
            repeats=[self.input_size_], 
            axis=1,
            name="game_phase_plane_width")

        one_hot_next_move_stone_types = tf.one_hot( # pylint: disable=no-value-for-parameter
            indices=next_move_stone_types, depth=3,
            name="one_hot_expand_next_move_stone_types")
        one_hot_next_move_stone_types = tf.reshape(
            tensor=one_hot_next_move_stone_types, 
            shape=[-1, 1, 1, 3],
            name="next_move_stone_types_create_channels")
        one_hot_next_move_stone_types = tf.repeat(
            input=one_hot_next_move_stone_types, 
            repeats=[self.input_size_], 
            axis=2,
            name="next_move_stone_types_plane_height")
        one_hot_next_move_stone_types = tf.repeat(
            input=one_hot_next_move_stone_types, 
            repeats=[self.input_size_], 
            axis=1,
            name="next_move_stone_types_plane_width")

        feature_planes = tf.concat( # pylint: disable=unexpected-keyword-arg, no-value-for-parameter
            values=[black_stones,
                    white_stones,
                    one_hot_game_phases,
                    one_hot_next_move_stone_types],
            axis=3,
            name="input_feature_planes")

        return feature_planes
